# Checking fmt-custom formatting optimization

I will refer to fmt v.7.0.0 implementation.

Let's assume we have the following custom type `data_t`:

```C++
struct data_t
{
    int x;
    int y;
    char c;
};
```

Let's introduce its formatting to fmt:

```C++
namespace fmt
{

template <>
struct formatter< ::test::data_t >
{
    using data_t = ::test::data_t;

    template < typename Context >
    constexpr auto parse( Context & ctx )
    {
        return ctx.begin();
    }

    template < typename Context >
    auto format( const data_t & d, Context & ctx )
    {
        return format_to( ctx.out(),
                          "[x:{}, y:{}, c:{}]",
                          d.x,
                          d.y,
                          d.c );
    }
};

} // namespace fmt

```

Fmt uses custom formatting with the following piece of code in [fmt/core.h](https://github.com/fmtlib/fmt/blob/5173a76ba49936d252a85ee49b7eb96e3dff4033/include/fmt/core.h#L944-L950):

```C++
  static void format_custom_arg(const void* arg,
                                typename Context::parse_context_type& parse_ctx,
                                Context& ctx) {
    Formatter f;
    parse_ctx.advance_to(f.parse(parse_ctx));   // ***
    ctx.advance_to(f.format(*static_cast<const T*>(arg), ctx));
  }
```

And it appears that in this case the line marked with `***` has no effect on the code produced by compiler with casual optimization flags. And it becomes the same as if it was `reduced_format_custom_arg()` function having no notion of parse formatting options:

```C++
  void reduced_format_custom_arg(const data_t d,
                                 typename Context::parse_context_type&,
                                 Context& ctx) {
        return format_to( ctx.out(),
                          "[x:{}, y:{}, c:{}]",
                          d.x,
                          d.y,
                          d.c );
  }
```

You can check it with the following [sample code](https://godbolt.org/z/5WeMK1) which shows the same assembly for true fmt-based case and for `reduced_format_custom_arg()` case. See the assemble under a line with formatter string - `.string "[x:{}, y:{}, c:{}]"`.

If you really-really want an unoptimizable `this` context for formatter then you would have to be very exlicit of your intention, like in this example:

```C++
namespace fmt
{
template <>
struct formatter< ::test::data_t >
{
    using data_t = ::test::data_t;

    template < typename Context >
    constexpr auto parse( Context & ctx )
    {
        return ctx.begin();
    }

    using iterator_t =
        std::back_insert_iterator< fmt::v6::detail::buffer< char > >;
    using context_t = fmt::v6::basic_format_context< iterator_t, char >;

    // Give compiler only a declareation:
    iterator_t format( const data_t & d, context_t & ctx );
};

} // namespace fmt
```

[Sample code](https://godbolt.org/z/v4MG6b).
