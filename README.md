[![release](https://img.shields.io/github/release/lemunozm/runtypes)](https://github.com/lemunozm/runtypes/releases)
[![license](https://img.shields.io/github/license/lemunozm/runtypes)](https://www.apache.org/licenses/LICENSE-2.0.txt)

# runtypes
*runtypes* is a lightweight C++ library that allows to create types in runtime.
The library has been made to bring the adventages of runtime with the less performance impact that runtime code comes with.

The aim of this library is to simplify the shared-memory applications offering
a fast, memory-efficient, and easy way to share data among process.
This not mean that this is the only purpose of the library. I can be used anywhere you need to define types in runtime.

## Getting started
Given the following C++ code,
```c++
struct Inner
{
    int id;
    std::map<int, float> values;
};

struct Outter
{
    std::string name;
    Inner inner;
};
```

you can easily create an **analogous code** that creates the same data structure in runtime using *runtypes*:
```c++
rt::Struct inner("Inner");
inner.add_member<int>("id");
inner.add_member<std::map<int, float>>("values");

rt::Struct outter("Outter");
inner.add_member<std::string>("name");
inner.add_member("inner", inner);
```

Once you defined the types, you can instantiate them and access to their data:
```c++
rt::data my_data(outter);

// write value
my_data["inner"]["id"].set(42);

// read value
const std::string& name = my_data["name"].get<std::string>();
```

## Why use *runtypes*?
* **C++11 API**: It uses the most modern features of C++11 to offer a really easy API to use.
* **Memory lightweight**: *runtypes* instantiations use the **same memory** as types builts by the compiler.
  There is no memory penalty using *runtypes* in relation to compiled types.
* **Fast**: small runtime cost accessing to data members.
* **Combine with any compiled type**: The type's members can be any *copyable* type/struct/class already compiled.
* **Header only library**: Fast and easy adaptation with you project, avoid linker issues and hard thirdparty management.
* **No external dependencies**: *runtypes* uses only tools from the `std`.
* **Easy to use**: Few classes, comprensive API and intuitive concepts.

## Build
*runtypes* is a header only library, you can simply copy the files located in the `include` folder into your project to make it works.

For a better management, *runtypes* comes as *CMake* project.
This allows you to add *runtypes* as thirdparty in your project and *"link"* against the library.

You can compile the library with tests and examples following the following commands:

```bash
$ git clone https://github.com/lemunozm/runtypes.git
$ cd runtypes && mkdir build && cd build
$ cmake -DRUNTYPES_BUILD_EXAMPLES=ON -DRUNTYPES_BUILD_TESTS=ON ..
$ make
```

Also, you can install the library in your computer to access easily from any project executing `make install`.

## API usage
*Examples can be found in [example folder](examples).*

### Type definition
Struct initialization:
```c++
rt::Struct my_struct;
rt::Struct my_named_struct("struct name");
```

To define your structs, you can use the `add_member` method.
This method has several usages:
* To create member with *c types*:
  ```c++
  my_struct.add_member("my_vector", std::vector<int>{1, 4, 9});
  ```
  This will create an `std::vector<int>` member called `my_vector` with default initial value of `{1, 4, 9}`.

  Also, you can initialize members without specify a value (default value/constructor):
  ```c++
  my_struct.add_member<std::string>("my_string"); // => ""
  ```

  Or you can emplace the object with an initial value calling a more complex constructor of your type:
  ```c++
  my_struct.add_member<std::string>("my_string_2", 'a', 5); // => "aaaaa"
  ```

* To reference already created structs.
  ```c++
  rt::struct previous_struct;
  my_struct.add_member("inner", previous_structs);
  ```
  Note: This member acts as a references, for that, the `previous_struct` lifetime must be longer than `my_struct`.

### Data manimulation
In order to instantiate data from a type is only necessary to call the data constructor:
  ```c++
  rt::Data my_data(my_type);
  ```

To access to the members in a `Struct` type, you can use the overload operator `[]`.
This will get a `WritableDataRef` or `ReadableDataRef` objects (depends if the access was `const` or not)
that allows to access to the internal data.
The `[]` operator can only be called from types that represents a `Struct`

To get the values of your *c types*, you can use the `get` and `set` methods.
This methods are template methods, so you need to specify the type you want to set/get.

* `get` method:
  The get method has two different usages:

  `const T& get() const` that will return the value:
  ```c++
  int i = data["outter_member"]["inner_member"].get<int>();
  ```

  `void get(T& t) const` that will return the value by reference:
  ```c++
  int i;
  data["outter_member"]["inner_member"].get(i); //type can be deducted as int
  ```
* `set` method:
  ```c++
  data["outter_member"]["inner_member"].set(6.7f); //type can be deducted as float
  ```

## Future work
* Adaptation to JSON, YAML and IDL formats: create types from these formats, and generate these format files from them.
* Serialization API to use common serialization standars easily.
* Comparative functions between types, optional members, etc...
