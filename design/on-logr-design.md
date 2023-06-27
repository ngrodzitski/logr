# On Logr Design

  * [Motivation](#motivation)
  * [Further thought and observations](#further-thought-and-observations)
     * [Amount of virtual calls](#amount-of-virtual-calls)
     * [Handling heavy messages](#handling-heavy-messages)
     * [Custom formatting](#custom-formatting)
     * [Separation of logging assisting code from application code](#separation-of-logging-assisting-code-from-application-code)
     * [Elimination of logging usage](#elimination-of-logging-usage)
  * [The approach](#the-approach)
     * [Summary of key design choice](#summary-of-key-design-choice)

## Motivation

Logging is a well-developed topic, and there are numerous libraries available that offer a wide range of logging approaches tailored to specific application needs.
However, this abundance of choices can create challenges when it comes to addressing logging requirements within a library, which is the issue that Logr aims to solve.

In the typical case of developing an application, choosing a logger is straightforward and limited to the application's codebase.
Once a logger library is selected, developers can follow their preferred logging practices without any issues.
However, when it comes to writing library software, the logger selection is not merely an internal concern.
A robust library is designed to be reused in various projects, which can be applications or other libraries themselves.
While many libraries may not require logging and could even be burdened by the introduction of logging functionality, let's focus on libraries that can provide added value through their log output.

This raises the problem: different clients of a library may adhere to different logging approaches.
One popular approach is to introduce a base logger class that clients can implement and then pass to the library routines.
I have personally employed this solution multiple times.
However, upon closer examination, certain issues emerge that may initially seem insignificant within the scope of a single project but become critical when considering multiple cases of library reuse.

Let's explore the trade-offs associated with a straightforward base logger-class approach.
As an illustrative example, we'll consider `FooLib`, which is used in `BarApp`.
In this scenario, `BarApp` has its own logger and wishes to utilize it within `FooLib` to track the internal state and events of `FooLib` (either as an optional or permanent feature).
While the following points may not be universally applicable to all cases, they represent some key considerations:

* The impact of virtual function calls.
* Handling of resource-intensive log messages. The process of building a message might be kind of resource-consuming.
* Custom log message formatting.

Additionally, I'd like to highlight a couple of personal concerns:


* Separation of logging assisting code from application-specific code.
* Enabling the ability to disable logging altogether (a feature commonly requested by developers).

Once again, for a single case in a single project, these considerations may not warrant the effort of developing a well-designed approach to minimize trade-offs.
However, when facing recurring tasks that share similar logical models, it becomes worth an effort to address these concerns properly at least for your conditions.

Therefore, an ideal solution should encourage consistent log usage and practices while imposing minimal additional costs associated with the aforementioned considerations.

## Further thought and observations

Let's shift our attention to the major concerns and thoroughly analyze them.
The objective is to gain a comprehensive understanding so that we are well-prepared to find an acceptable solution.

### Amount of virtual calls

The base logger-class approach assumes the existence of a virtual function, such as `virtual mb_return_type_t log_message(msg) = 0;`, which is implemented by derived classes.
However, in scenarios with heavy logging and frequent events, the presence of virtual calls can have a negative impact on overall performance.
Depending on the given conditions, it may or may not be possible to eliminate or reduce the number of virtual calls.

For instance, if `FooLib` is a header-only library and the logger type is a template parameter, the compiler can potentially devirtualize all calls to the `log_message()` function and enable inlining.
This optimization is especially likely if the logger implementation is marked as `final` or if the target functions are `final`.
You can refer to a [short demo](https://godbolt.org/z/KvnhfW) showcasing what the compiler can do for devirtualization.

In a non-header-only context, completely eliminating virtual calls is typically not feasible, as the `FooLib` code lacks knowledge of the specific logger type, except when using the exact "final" type of logger.
Therefore, the goal should be to minimize the number of virtual calls.
Let's explore some considerations that might help us achieve this optimization.

Here are a few considerations:

* Log level (severity of a message): If a logger checks the severity of a message before processing it, it can be implemented as a separate routine outside of the `log_message()` virtual function.
  By doing so, it can be inlined, allowing for additional optimizations. For example, heavy operations can be avoided if it is known in advance that the resulting message won't be consumed.
* Formatting or message preparation: Let's narrow our focus to formatting using the [fmt](https://github.com/fmtlib/fmt) library (which strives to become part of the standard, making it more widely applicable).
  What happens if we perform message formatting before invoking `log_message()`? When a call to a non-devirtualizable virtual function is present, it can limit compiler optimizations.
  The compiler can perform optimizations on the code before and after the call, or it can optimize the code inside the call (I would appreciate if someone would tell me of optimizations to the code with the call in the middle).
  So, we have a piece of formatting code that can be placed inside the `log_message()` virtual function or outside of it. By carefully choosing one of these options, we may gain certain benefits.

### Handling heavy messages


Log messages often involve the rendering some of the applications data to text, which incurs a certain cost.
Let's explore the factors that contribute the most to the cost of constructing a log message:

1. Formatting itself - the way the data is being put into text. I am aware of the following approaches:

    * Stream-like formatting (e.g. [glog](https://github.com/google/glog), [log4cplus](https://github.com/log4cplus/log4cplus), [Boost.Log](https://www.boost.org/doc/libs/1_78_0/libs/log/doc/html/index.html)).
    * Fmt-like formatting (e.g. [spdlog](https://github.com/gabime/spdlog)). The formatting framework is not necessary must be fmtlib, but the core idea is the same (we can also call it printf-formatting, which has the same idea and historically is the prior one).
    * Fixed-messages. A set of messages with its input parameters is fixed and known. Logger-lib provides a routine to generate a thing that would accept messages data as a function accepts arguments.
      It can be a set of classes with member functions or a set of free functions corresponding to log-messages.
      The internal rendering doesn't affect the user and can be any of the previously mentioned or even don't exist as a text-rendering but be some kind of binary rendering.
      Though this is not a widely spread approach, and I can't name a modern and publicly available library implementing it.
      I just find it interesting and deserving at least an honorable mention.

2. Text Storage: Text storage refers to the data structure that holds the message data during its construction phase (when new pieces of text are being appended) and serves as the carrier for the message data to the logger's consumer.
   Key characteristics of text storage include:
   * Allocation properties.
   * Movability properties. This affects the efficiency of the carrier function for the storage.

When selecting among different options, it is essential to consider the message profile of your application to ensure that the trade-offs are acceptable.
Furthermore, it would be advantageous to have an approach that performs rendering and storage only when necessary, meaning that the message will be consumed.
This optimization helps to avoid unnecessary overhead when logging is not enabled or when messages are not intended to be consumed.

### Custom formatting

Custom formatting, also known as formatting for user-defined types, is not a mandatory feature, as it can be implemented inline within the available formatting approaches (stream-like or fmt-like).
However, the common practice of having both stream-like and fmt-like formatting providing natural ways to implement custom formatting deserves closer consideration to understand the trade-offs of different approaches.

In the stream-like approach, custom formatting is essentially just an overload for `operator<<(out, obj)` where `out` is a generic output stream provided by the logger library and `obj` is a user-defined C++ type.

For fmt-like approach, please refer to the [fmt documentation on formatting user-defined types](https://fmt.dev/latest/api.html#formatting-user-defined-types).

In fmt-like formatting, an additional function called `constexpr auto parse(format_parse_context& ctx)` allows you to create options to control the formatting behavior.
Ignoring formatting options for now, both approaches described above are equivalent.

Essentially, we have a function that operates on references to the output context (a stream in the case of stream-like formatting) and the target user-defined type.
This statement requires checking because, in fmt-like custom formatting, it involves a struct with member functions.
However, with intelligent optimizers, there is no practical difference in similar cases. Unused abstractions collapse to nothing, so you don't pay for what you don't use.
Please refer to the [fact-checking](./checking-fmt-custom-formatting-optimizations.md) for more information.

When considering the control flow of the message building process and the need to inject a custom formatting routine, we need to think about how this routine fits into the overall message building process.
One concern is how to transfer the rendering context between the caller and the callee.

It is not ideal to have a local `std::string` variable and construct a textual representation of the custom type as a string before using it (since strings are commonly used for formatting any type).
Both stream-like and fmt-like formatting address this concern by passing the output context to the formatting routine.
Stream-like formatting offers a shift operator overload approach, which leaves not too much space to do it wrong.
In the case of fmt, one can provide template specialization for the `fmt::formatter<T>` class or use the shift operator overload (refer to `<fmt/ostream.h>` for details).

### Separation of logging assisting code from application code

Let me clarify what I mean.
The separation of logging assisting code from application code can be beneficial in scenarios where heavy operations, such as database queries or lookups/searches, are required to generate informative log messages.
While this may not seem like a significant issue, it becomes a concern when heavy operations are performed for trace/debug messages that are not part of the standard application run.
Ideally, you wouldn't want to perform heavy operations for messages that won't occur, and neither do I.

To address this issue, one approach is to provide a conditional block of code - not beautiful and eventually makes a double-checking of the log level (first to decide whether to do heavy stuff and the second check happened when asking logger to log the message).
One way to address this issue is to provide a function-entity (free function, lambda, or any other callable) that can fit into the message construction process and operate over output context, but it still requires a separate boilerplate and abstraction (interestingly, it becomes a reasonable tradeoff in case of multiple reuses).
Another approach is to utilize macros to avoid double-checking. However, with this approach, you have less flexibility in performing normal function-like operations since you are constrained within the 
ostream-operators chain.

Ideally, it would be beneficial to have logging assistance functionality confined within the scope of the logger itself. Consider the following examples:

```C++
logger.msg_xxx( /*logger territory*/ );
// Or
LOG(INFO, /*logger territory*/) << /*logger territory*/;
```

The scope of the logger is `/*logger territory*/`. So the intention is not to pollute application code with logging semantics:

```C++
// Somewhere in codebase 1 (Glog/Boost-like):

auto make_associated_record = [&]{
    // Do a couple of lookups to get and translated ids to human-readable data
    // and construct the full record data.
    return full_record_t{ /*...*/ };
};

// A lambda object - `make_associated_record` 
// is used only for logging but lives to the end of a current scope.
LOG(DEBUG) << "Handling the record: " << make_associated_record();

// Somewhere in codebase 2 (spdlog-like):

if( logger.needs_log( the_debug ) ) // Checks logging level.
{
    // Do a couple of lookups to get and translated ids to human-readable data
    // and construct the full record data.

    // Checks logging level again.
    logger.debug( "Handling the record: {}", full_record_t{ /*...*/ } );
}

// Somewhere in codebase 3 (spdlog-like another approach).
// If implementation of `make_associated_record()`
// is not visible to compiler it most likely will choose
// to call the routine first (maybee under some conditions it will
// eliminate function call prior call, but is not a very reliable thing)
// and then run `logger::debug( ... )` logic which is bad if
// log_level is "above" debug.
logger.debug( "Handling the record: {}",
              make_associated_record( p1,p2, /*...*/ ) );
```

### Elimination of logging usage

That what most of the logger libraries do pretty well by adjusting implementation via proper macro definitions.

## The approach

Having a collection of concerns and issues enlightened above, let's see how they can be handled, and let's build an approach to bring the balance to a possible solution in a most satisfactory way.

A couple of terms:

* Log-statement refers to a piece of code that is somehow linked to a single message logging:

```C++
// Examples of log-statements (depends on the logger API):
LOG(ERRROR) << "oops: " << ex.what();

logger.info( "Simple log-statement: {}, {}", x, y );

LOG_INFO(logger, "Also log statement: " << str);
```

* Log-virtual-call refers to a call of a routine consuming the log message.

To minimize the number of log-virtual-calls and eliminate those of them that are unnecessary we should keep all decisions logic of the logger in headers preferably without any virtuality and let the compiler to decide what optimization it can apply to code pieces of loggers decision logic and the outer context where the logger is used. Also, we should trigger at most 1 virtual call per single log-statement, while the routine behind the call might be responsible for other indirect calls in the scope of a given log-statement, it should be not more than one explicit virtual call. It also would be nice to have a virtual call (if it happens) as the last thing in the execution sequence that is generated for a log-statement. It would allow putting function call-barrier, which breaks many compiler optimizations to the tail of the generated code. The last consideration requires all formatting to happen before log-virtual-call, which brings the idea of splitting the task performed by log-statements into two pieces - the first one prepares the message and the second one distributes it to final destinations.

**Key design choice 1:**
the separation of routines: all message preparations (including deciding whether a given message should happen) and message consumption. We can refer to the first part as the frontend and the second as the backend.

* Frontend is responsible for message preparation:

    * Deciding whether to log this message;
    * Handle message formatting;
    * Pass the prepared message to the backend.

* Backend is responsible for directing the message to the destination. A piece of logic that usually distributes messages to a set of sinks (a common term for a thing representing final log message destination, e.g. file).

Now let's think of message formatting. Spdlog library shows the power of fmt as a text rendering engine for logging (IIRC, I've come across spdlog first, and only then I had discovered fmt for me). So fmt for formatting is an excellent choice for a casual case (your mileage might vary).

**Key design choice 2:**
use [fmt](https://github.com/fmtlib/fmt) for formatting. This choice directly implies the custom formatting approach provided by fmt.

If fmt is already here (as a 3rd-party lib) to address the issue of temporary message storage, it is reasonable to use "smart" [buffer](https://fmt.dev/latest/api.html#formatclassfmt_1_1basic__memory__buffer) provided by fmt. It can be adjusted so that most messages would be handled without memory allocation and would use only a "small" stack buffer supplied by the smart-buffer itself.

"Smart" [buffer] is good for allocations, but it hurts movability. Let's see if it can be an acceptable tradeoff. It is known that a message constructed in log-statement usually is not ultimately final, and it is more correct to call it message body. When receiving a message body backend adds a timestamp, log-level, and other standard attributes. So the move semantics here is likely to draw not so much profit. If moving message data is a concern (e.g. case of pushing messages to a queue and handling it in another thread) backend can pack the message into a movable thing. In the case of adding "standard" attributes to a message, the backend already needs to provide storage so it's up to it to choose a proper approach. The uncomfortable conditions would be the following: backend adds more attributes but not assembles the ultimately final message but wants to move the data to queue (handled in a separate thread). The absence of movability here would require a copy and maybe extra allocations for new storage. Happily (for me), the later case is not mine, so I'm OK with using an unmovable stack buffer.

**Key design choice 3:**
Use `string_view` to pass a prepared message body from frontend to backend (message-data carrier).

Well, there is another objective I was trying to address with Logr:
separation of logging assisting code from application code. And here comes the following design choice.

**Key design choice 4:**
The idea of message builders. The frontend receives a callback that creates a message if necessary. The standard case would be using lambda as a message builder.

With lambda, it is possible to access the outer context (to get the values necessary to build a message) while introducing own scope thus keeping the outer nesting scope clean from possible artifacts of message preparation.

```C++
int id_one,id_two;
// ...
logger.info( [&]( auto & out_ctx ){
    // Do a couple of lookups to get and translated ids to human-readable data
    // and construct the full record data.
    full_record_t rec{ /*...*/ };

    format_to( out_ctx, "{}:{} = > {}", id_one, id_two, rec );
} );
```

```C++
// Conditional message alternation happens inside
// message builders, so it happens only if the log-statement
// passes log level check:
    int x,y;
    // ...
    logger.info( [&]( auto & out_ctx ){
        if( 0 == x || 0 == y || max_x == x || max_y == y )
        {
            // Make it easier for humans to see edge cases in a log.
            format_to( out_ctx, "handling ({}:{}) THE EDGE CASE", x, y );
        }
        else
            fmt::format_to( out_ctx, "handling {}:{}", x, y );
    } );
```

### Summary of key design choice

* Frontend backend separation.
* Use [fmt](https://github.com/fmtlib/fmt) for formatting.
* Use `string_view` to pass a prepared message body from frontend to backend.
* A concept of message builders.
