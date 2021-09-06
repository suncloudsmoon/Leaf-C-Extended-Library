# Leaf C Extended Library
Leaf C Extended Library is a simple and easy to use C/C++ library that allows programmers to safely use strings, cryptography, and more while maintaining excellent performance
and portability!

# Strings
With our string library, you can use dynamically allocated strings with less pain because the string library automatically reallocates more memory when necessary
and you can increase the optimization level of the amount of reallocation to allocate more than necessary characters. As a bonus, it is possible to use the string lib without
malloc'ing it too!

You: Ok. How can I use it?

A very simple way to use the library dynamically...
```
jtr_t *str = malloc(sizeof(jtr_t));
str->optimization_level = 0.5; // optional
jtrnew(str, "Hello ");
jtrcat(str, "World!");
printf("Example String: %s\n", str->text);
jtrfree(str);
free(str);
```

Without malloc:
```
jtr_t str = {0};
str.optimization_level = 0.5; // optional
jtrnew(&str, "Hello ");
jtrcat(&str, "World!");
printf("Example String: %s\n", str.text);
jtrfree(str);
```

# Crypto
Sorry, I have not added anything to crypto yet :/

# Installing
Just copy the include/leaflib/... into your project and you are good to go (don't need to worry about license bc it uses MIT-0 (no attribution)).

# Features
- Has very high interoperability with C strings
- Can specify the optimization level for each string struct (whether to realloc more in advance if the string is going to get very big)
- High Performance
- Very small
- Highly Portable

# Upcoming Features
- Add similar functionality to "sprintf" in this library

