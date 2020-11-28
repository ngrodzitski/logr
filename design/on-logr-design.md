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

Logging is a well-developed topic, you can find lots of libraries suggesting a rich specter of approaches tuned for various application specifics. Such a variety in some sense causes the issue I'm trying to address here with Logr.

In the casual case of making a piece of application software choosing a logger has a well-defined scope of usage - it is the application code. Once the choice of logger a library is made (it can also be your in-house one), you follow a practice that you find appropriate for it. And there is nothing wrong with it.

However, choosing a logger while writing library software is not purely an internal affair. A good library is intended to be reused in other projects, that can be applications or libraries by themselves. There are lots of libraries that don't need any logging and would even lose if an effort to introduce one will be taken. But let us focus on libraries giving an extra value if you can get its log.

And here comes the problem: different clients of your library might stick to different logging approaches. A popular approach is to introduce a base logger class that can be implemented on the client-side and then passed to the library routine. And I've applied that kind of solution many times. Digging into the details of such practice some issues might seem unimportant on the scale of a single project but become subjects to consider while thinking at the scale of multiple cases of reuse.

Let's see what are the trade-offs of a straightforward base logger-class approach. For the narrative, as a reference example, I will use FooLib that is used in BarApp. So BarApp has its logger and wants to use it in FooLib to track FooLib's internal state and events (an optional or a permanent feature). Without claiming to be universally valid for all the possible cases major points would be the following:

* The number of virtual calls.
* Handling heavy messages. The process of building a message might be a kind of resource consuming.
* Custom formatting.

A bonus couple of points (my subjective concerns):

* Separation of logging assisting code from application code.
* Elimination of logging usage.

Once again for a single case in a single project, those considerations might not be sufficient to accept the efforts on building a well-designed approach to minimize trade-offs associated with the mentioned concerns. But when the similar (in terms of its logical model) task arises again and again you would be thinking about how to deal with it properly at least for your conditions.

So a good solution would try to enforce log usage and practice that draws minimal extra cost associated with the above-mentioned considerations under given conditions.

## Further thought and observations

Let's focus on the major concerns and analyze them. The goal is to gather a detailed picture so we would be prepared to find an acceptable solution.

### Amount of virtual calls

Base logger-class approach assumes some king of the following virtual function: `virtual mb_return_type_t log_message( msg ) = 0;` which is provided by implementations. In event-intensive scenarios with lots of logging, it puts a negative effect on overall performance. Under given conditions, the virtual call can be eliminated or can be unavoidable.

For example, if FooLib is a header-only library and logger type is a template parameter then the compiler can devirtualize all calls to `log_mesage()` function and allowing inlining (at least it will try to), especially if logger implementation is marked as `final` (or target functions are `final`).

See a [short demo](https://godbolt.org/z/KvnhfW) of what compiler can do for devirtualization.

In a non-header-only context, it most likely can't be reduced to zero as no knowledge of a true logger does exist on the level of the FooLib cod, an exception would be using the exact "final" type of logger. So the goal might be to minimize the number of virtual calls. Let's see, maybe some logging domain specifics can come to the rescue.

Here are some considerations:

*  Log level (severity of a message). If a logger checks message severity before consuming it then it looks like a simple checker routine which can be moved outside of a `log_mesage()` virtual function and be inlined to allow more optimizations e.g. to prevent heavy stuff to execute if it is just known that the resulting message won't be consumed.

* Formatting or message preparation. Let's narrow the case to formatting with [fmt](https://github.com/fmtlib/fmt) library (which tries to make its core into the standard, so maybe it is not so narrow look as it seems at first). What if we do message formatting before `log_mesage()` is called? A call to a non-devirtualizable virtual function puts a limitation on compiler optimizations - it can perform a bunch of optimizations with the local code before the call and after it or on the other hand, it can be optimized with the stuff inside the call - inside the implementation (I would appreciate if someone would tell me of optimizations to the code with the call in the middle.). So we have a piece of formatting code and it can be placed inside the implementation of `log_mesage()` virtual function or outside of it. And maybe we can have benefits deciding one of the options?

### Handling heavy messages

Log messages are mostly concerned with rendering application data to text and that has its cost. Here are some points contributing the most to the cost of building a log message:

1. Formatting itself - the way the data being put to text. I am aware of the following approaches:

    * Stream-like formatting (e.g. [glog](https://github.com/google/glog), [log4cplus](https://github.com/log4cplus/log4cplus)).
    * Fmt-like formatting (e.g. [spdlog](https://github.com/gabime/spdlog)). The formatting framework is not necessary must be fmtlib, but the core idea is the same (also it can be called printf-formatting which has the same idea and historically is the prior one).
    * Fixed-messages. A set of messages with its input parameters is fixed and known. Logger-lib provides a routine to generate a thing that would accept messages data as a function accepts arguments. It can be a set of classes with member functions or a set of free functions corresponding to log-messages. The internal rendering doesn't affect the user and can be any of the previously mentioned or even don't exist as a text-rendering but be some kind of binary rendering. Though this is not a widely spread approach and I can't name a modern and publicly available library implementing it I find it interesting and deserving at least an honorable mention.

2. A storage for text. By storage I mean the thing that is capable of keeping a message-data during its construction phase (while you or the lib appending new pieces of text to it) and the thing (it might be a different thing) that serves as a carrier of message-data to whatever consumer provided by logger is. Important characteristics of text storage are:

    * Allocation properties.
    * Movability properties. It affects the efficiency of the carrier function of the storage.

Deciding between different options one should account for applications messages profile to be sure trade-offs made are acceptable.

Also, it would be great to provide an approach that does the rendering and storage only if it is really necessary, it means the message will be consumed.

### Custom formatting

Also referred to as the formatting of user-defined types.

I can't say it is a mandatory feature. In any case, one can just implement it inlined within the available formatting approach (stream-like or fmt-like). But having stream-like formatting and fmt-like formatting both providing a natural way to implement custom formatting - it's just so common that this practice deserves a closer look to find out what are the tradeoffs for different approaches.

In stream-like approach custom formatting is essentially just an overload for `operator<<(out, obj)`, where `out` is a generic output stream provided by logger library and `obj` is some application type.

For fmt-like approach, please, refer to [fmt-doc (formatting user-defined types)](https://fmt.dev/latest/api.html#formatting-user-defined-types).

Fmt-like formatting has an extra function involved called `constexpr auto parse(format_parse_context& ctx)` which makes it possible to create your options to control formatting behavior. Leaving formatting options out of further discussion both approaches described above are equivalent.

So basically we have a function operating over references to output context (for stream-like formatting it is a stream itself) and target user-defined type. This statement requires checking. Because for fmt it is not strictly correct. In fmt-like custom formatting, it is a struct with member functions that does the trick. But having clever optimizers it makes no difference in similar cases. So you don't pay for what you don't use and unused abstraction collapses to nothing. Please, refer to the following [fact-checking](./checking-fmt-custom-formatting-optimizations.md).

So if we consider a control flow of a process of building a message and we want to inject our formatting routine (or call it logic or a piece of code) appending our custom type text rendering we should think of how this routine fits into a given position of the message building process. And I can't name anything besides the following concerns: how to transfer rendering context between the caller and the callee.

It is not a great idea to have a local `std::string` variable and construct a textual image of a custom type to it first and later use it (as strings are well known to any type of formatting). Both stream-like and fmt-like formatting do a great job with this concern by passing output context to formatting routine. Stream-like formatting offers a shift operator overload approach and there is not too much space to do it wrong. In fmt case, one should provide template specialization for `fmt::formatter< T >` class or also provide shift operator overload (refer to `<fmt/ostream.h>` for details).

### Separation of logging assisting code from application code

Let me clarify what I mean. It might be required to do heavy stuff (like DB query or a search or a lookup) to give a more informative log message. Looks like not a very big issue. But here what draws my attention to it. A couple of times I got it for trace/debug messages and these messages are not a subject of the standard application run. And obviously, you don't want to do heavy stuff to assist a message that won't happen and neither do I. So I had to have a conditional block of code which adds no beauty and eventually have a double-checking of the log level (first to decide whether to do heavy stuff and the second check happened when asking logger to log the message). One way to address this issue is to provide a function-thing (free function, lambda, or any other callable) that can fit into the message construction process and operate over output context, but it still requires a separate boilerplate and abstraction (interestingly it becomes a reasonable tradeoff in case of multiple reuses). Another way would be using macros machinery to avoid double-checking.

So it would be nice to have a logging assistance stuff trapped in logger scope, e.g. consider this:

```C++
logger.msg_xxx( /*logger territory*/ );
// Or
LOG(INFO, /*logger territory*/) << /*logger territory*/;
```

The scope of the logger is `/*logger territory*/`. So the intention is not to pollute application code with logging semantics:

```C++
// Somewhere in codebase 1 (Glog-like):

auto make_associated_record = [&]{
    // Do a couple of lookups to get and translated ids to human-readable data
    // and construct the full record data.
    return full_record_t{ /*...*/ };
};

// `make_associated_record` used only for logging
// but live to the end of a current scope.
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

Having a collection of concerns and issues enlightened above let's see how they can be handled and let's build an approach to bring the balance to a possible solution in a most satisfactory way.

A couple of terms:

* Log-statement refers to a piece of code that is somehow linked to a single message logging:

```C++
// Examples of log-statements (depends on the logger API):
LOG(ERRROR) << "oops: " << ex.what();

logger.info( "Simple log-statement: {}, {}", x, y );

LOG_INFO(logger, "Also log statement: " << str);
```

* Log-virtual-call refers to a call of a routine consuming the log message.

To minimize the number of log-virtual-calls and eliminate those of them that are unnecessary we should keep all decisions logic of the logger in headers preferably without any virtuality and let the compiler to decide what optimization it can apply to code pieces of loggers decision logic and the outer context where the logger is used. Also, we should trigger at most 1 virtual call per single log-statement, while the routine behind the call might be responsible for other indirect calls in the scope of a given log-statement it should be not more than one explicit virtual call. It also would be nice to have a virtual call (if it happens) as the last thing in the execution sequence that is generated for a log-statement. It would allow putting function call-barrier which breaks many compiler optimizations to the tail of the generated code. The last consideration requires all formatting and to happen before log-virtual-call, which brings the idea of splitting the task performed by log-statements into two pieces - the first one prepares the message and the second one distributes it to final destinations.

**Key design choice 1:**
the separation of routines: all message preparations (including deciding whether a given message should happen) and message consumption. We can refer to the first part as the frontend and the second as the backend.

* Frontend is responsible for message preparation:

    * Deciding whether to log this message;
    * Handle message formatting;
    * Pass the prepared message to the backend.

* Backend is responsible for directing the message to the destination. A piece of logic that usually distributes messages to a set of sinks (a common term for a thing representing final log message destination, e.g. file).

Now let's think of message formatting. Spdlog library shows the power of fmt as a text rendering engine for logging (IIRC I've come across spdlog first and then had discovered fmt for me). So fmt for formatting is a great choice for a casual case (your mileage might vary).

**Key design choice 2:**
use [fmt](https://github.com/fmtlib/fmt) for formatting. This choice directly implies the custom formatting approach provided by fmt.

If fmt is already here (as a 3rd-party lib) to address the issue of temporary message storage it is reasonable to use "smart" [buffer](https://fmt.dev/latest/api.html#formatclassfmt_1_1basic__memory__buffer) provided by fmt. It can be adjusted so that the majority of messages would be handled without memory allocation and would use only "small" stack buffer provided by the buffer itself.

"Smart" [buffer] is good for allocations but it hurts movability. Let's see if it can be an acceptable tradeoff. It is known that message constructed in log-statement usually is not ultimately final it is more correct to call it message body. Taking a message body backend adds a timestamp, log-level, and other standard attributes. So the move semantics here is likely to draw not so much profit. If moving message data is a concern (e.g. case of pushing messages to a queue and handling it in another thread) backend can pack the message into a movable thing. In the case of adding "standard" attributes to a message, the backend already needs to provide storage so it's up to it to choose a proper approach. The uncomfortable conditions would be the following: backend adds more attributes but not assembles the ultimately final message but wants to move the data to queue (handled in a separate thread) the absence of movability here would require a copy and maybe extra allocations for new storage. Happily, for me, the later case is not mine, so I'm fine with using an unmovable stack buffer.

**Key design choice 3:**
Use `string_view` to pass a prepared message body from frontend to backend (message-data carrier).

Well, there is another objective I was trying to address with Logr:
separation of logging assisting code from application code. And here comes the following design choice.

**Key design choice 4:**
The idea of message builders. The frontend can receive a callback that creates a message if necessary. The standard case would be using lambda as a message builder.

With lambda, it is possible to access the outer context (to get the values necessary to build a message) while introducing own scope thus keeping the outer nesting scope clean from possible artifacts of message preparation.

```C++
int id_one,id_two;
// ...
logger.info( [&]( auto & out_ctx ){
    // Do a couple of lookups to get and translated ids to human-readable data
    // and construct the full record data.
    full_record_t rec{ /*...*/ };

    fmt::format_to( out_ctx, "{}:{} => {}", id_one, id_two, rec );
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
            fmt::format_to( out_ctx, "handling ({}:{}) THE EDGE CASE", x, y );
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
