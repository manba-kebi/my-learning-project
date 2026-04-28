## 链接

- 这是他的教程页[CMake教程]([CMake 教程 — CMake 4.3.2 文档 --- CMake Tutorial — CMake 4.3.2 Documentation](https://cmake.org/cmake/help/latest/guide/tutorial/index.html))，详情可以自己看看（有点多）

- 这是下载地址[下载链接]([下载 CMake --- Download CMake](https://cmake.org/download/))
- 文件夹中的**cmake-4.3.2-tutorial-source.zip**是官网上的教程源代码示例，链接在这里：[下载代码示例链接](https://cmake.org/cmake/help/latest/_downloads/617976f122c88ee3ce900143cd0544eb/cmake-4.3.2-tutorial-source.zip)



## 看之前准备的话

说实话我这篇其实有挺多内容没写上的，就比如教程中提到的教程源代码相关部分，我基本没写。我觉得这个**官方页面设计的很用心在教程源代码这一部分**，真心推荐大家可以跟着官方文档看看。毕竟在这篇报告中所提到的一些内容还有可能是我总结出来的。仁者见仁智者见智，我也不可能是最好的，自己看看说不定还会获得新的感悟。

这是我在看完CMake教程后想对大家说的：我这篇文档中基本都是对源教程的翻译，而且还缺少例题部分，推荐大家去看看源文档。



# 第一节 开始使用CMake

## 背景

CMake 的典型使用是围绕一个或多个名为 `CMakeLists.txt`。该文件有时被称为“列表文件”或“CML”。在给定的软件项目中，任何目录中都会存在一个 `CMakeLists.txt`，我们希望向 CMake 提供如何处理该目录或子目录本地文件和操作的指令。每个命令由一组描述与构建软件项目相关的信息或操作组成。



并不是软件项目中的每个目录都需要 CML，但强烈建议项目根包含 CML。这将作为 CMake 在配置时初始设置的入口。这个*根* CML 应始终包含文件顶部或顶部附近的两个命令。



```c++
cmake_minimum_required(VERSION 3.23)

project(MyProjectName)
```

[`cmake_minimum_required（）`](https://cmake.org/cmake/help/latest/command/cmake_minimum_required.html#command:cmake_minimum_required) 是 CMake 向项目开发者提供的兼容性保证。调用时，它确保 CMake 会采用所列版本的行为。如果在包含上述代码的 CML 上调用了后续版本的 CMake，其行为将完全如同 CMake 3.23。

[`project（）`](https://cmake.org/cmake/help/latest/command/project.html#command:project) 命令是一个概念简单的命令，但功能复杂。它通知 CMake，接下来是对一个特定名称的独立软件项目的描述（而非类似 shell 的脚本）。当 CMake 看到 [`project（）`](https://cmake.org/cmake/help/latest/command/project.html#command:project) 命令时，会进行各种检查以确保环境适合构建软件;例如检查编译器和其他构建工具，发现主机和目标机的端序等属性。



## 构建可执行文件

我们已经介绍的前两个命令，`cmake_minimum_required（）`和`project（）`。如果根CML中的第一个命令不是`CMake_minimum_required（）`，则不使用`CMake`。在某些高级用法中，`project（）`可能不是CML中的第二个命令，但就我们的目的而言，它始终是。



### [`add_executable（）`](https://cmake.org/cmake/help/latest/command/add_executable.html#command:add_executable)

这个命令会创建一个*目标* 。在 CMake 的术语中，目标是开发者给一组属性命名的。

目标可能想要跟踪的一些属性示例包括：

- 工件类型（可执行文件、库、头部集合等）
- 源文件
- 包含目录
- 可执行文件或库的输出名称
- 依赖关系
- 编译器和链接器标志

标本身就是名称，是这组属性的句柄。使用 [`add_executable（）`](https://cmake.org/cmake/help/latest/command/add_executable.html#command:add_executable) 命令只需指定目标的名称即可。

```c++
add_executable(MyProgram)
```



### [`target_sources（）`](https://cmake.org/cmake/help/latest/command/target_sources.html#command:target_sources)

现在我们有了目标的名称，就可以开始将属性与它关联，比如我们想构建和链接的源文件。主要命令是 [`target_sources（）`](https://cmake.org/cmake/help/latest/command/target_sources.html#command:target_sources)，它以目标名和一个或多个文件集合作为参数。

```c++
target_sources(MyProgram
  PRIVATE
    main.cxx
)
```





###  [`add_library（）`](https://cmake.org/cmake/help/latest/command/add_library.html#command:add_library)

我们只需要再引入一个命令来构建一个库，即 [`add_library（）`](https://cmake.org/cmake/help/latest/command/add_library.html#command:add_library)。这与 [`add_executable（）`](https://cmake.org/cmake/help/latest/command/add_executable.html#command:add_executable)的工作方式完全相同，但适用于库。

```c++
add_library(MyLibrary)
```



然而，现在是引入头文件的好时机。头文件不是直接作为翻译单元构建的，也就是说它们不是构建要求。它们是使用要求。我们需要了解头文件，以便构建给定目标的其他部分。

因此，头文件的描述方式和像 `tutorial.cxx` 这样的实现文件略有不同。它们也需要不同的 范围[关键词 ](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#target-command-scope)，而不是我们迄今为止使用的 `PRIVATE` 关键词。

为了描述一组头文件，我们将使用所谓的 `FILE_SET`。

```c++
target_sources(MyLibrary
  PRIVATE
    library_implementation.cxx

  PUBLIC
    FILE_SET myHeaders
    TYPE HEADERS
    BASE_DIRS
      include
    FILES
      include/library_header.h
)
```



这很复杂，但我们将逐一介绍。首先，请注意，我们的**实现文件是`PRIVATE`源代码，和前面的可执行文件一样**。但是，**我们现在使用`PUBLIC`作为头文件**。**这允许我们库的消费者“看到”库的头文件。**



`scope`关键字后面是`FILE_SET`，这是一个被描述为单个单元的文件集合。`FILE_SET`由以下部分组成：

- `FILE_SET <name>` 是 `FILE_SET` 的名称。这是一个句柄，我们可以在其他上下文中使用它来描述集合。
- `TYPE <type>` 就是我们描述的文件类型。最常见的是头部文件，但新版本的 CMake 支持其他类型，比如 C++20 模块。
- `BASE_DIRS`是文件的“基本”位置。这最容易理解为将通过`-I`标志向编译器描述用于头发现的位置。
- `FILES` 是文件列表，与之前的实现源代码列表相同。

这些信息量很大，需要详细说明，所以我们可以采取一些实用的捷径。值得注意的是，如果 `FILE_SET` 名和类型相同，我们就不需要提供 `TYPE` 字段。：

```c++
target_sources(MyLibrary
  PRIVATE
    library_implementation.cxx

  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      include
    FILES
      include/library_header.h
)
```



### [`target_link_libraries（）`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html#command:target_link_libraries)

我们已经准备好将我们的库与可执行文件相结合，为此，我们必须引入一个新命令`target_link_libraies（）`。这个命令的名称可能会有点误导，因为它不仅仅是调用链接器。它通常描述目标之间的关系。

```c++
target_link_libraries(MyProgram
  PRIVATE
    MyLibrary
)
```

我们终于准备好讨论[范围关键词](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#target-command-scope)了。有三个关键词，分别是 `PRIVATE`、`INTERFACE` 和 `PUBLIC。` 这些关键词描述了属性如何向目标开放：

- `PRIVATE`属性（也称为“非接口”属性）**仅对拥有它的目标可用**，例如PRIVATE标头仅对它们所附加的目标可见。
- `INTERFACE`属性**仅对链接所属目标的目标可用**。**拥有这些属性的目标无权访问**。仅标头库是INTERFACE属性集合的一个示例，因为仅标头库本身不构建任何内容，也不需要访问自己的文件。
- `PUBLIC`不是一种独特的属性，**而是`PRIVATE`和`INTERFACE`属性的结合**。因此，`PUBLIC`描述的要求**对拥有目标和消费目标**都是可用的。

请考虑以下具体例子：

```c++
target_sources(MyLibrary
  PRIVATE
    FILE_SET internalOnlyHeaders
    TYPE HEADERS
    FILES
      InternalOnlyHeader.h

  INTERFACE
    FILE_SET consumerOnlyHeaders
    TYPE HEADERS
    FILES
      ConsumerOnlyHeader.h

  PUBLIC
    FILE_SET publicHeaders
    TYPE HEADERS
    FILES
      PublicHeader.h
)
```

*注意 ：我们为每个文件集排除`了 BASE_DIRS`，这又是一个快捷方式。排除后，`BASE_DIRS` 默认使用当前的源目录。*

`MyLibrary` 目标有若干属性，将通过调用 [`target_sources（）`](https://cmake.org/cmake/help/latest/command/target_sources.html#command:target_sources) 进行修改。直到现在，我们一直泛指“属性”一词，但属性本身就是我们可以推理的命名值。这里将修改的两个具体属性是 [`HEADER_SETS`](https://cmake.org/cmake/help/latest/prop_tgt/HEADER_SETS.html#prop_tgt:HEADER_SETS) 以及 [`INTERFACE_HEADER_SETS`](https://cmake.org/cmake/help/latest/prop_tgt/INTERFACE_HEADER_SETS.html#prop_tgt:INTERFACE_HEADER_SETS)，这两个都包含通过 [`target_sources（）`](https://cmake.org/cmake/help/latest/command/target_sources.html#command:target_sources) 添加的头部文件集列表。

值 `internalOnlyHeaders` 将被添加到 [`HEADER_SETS`](https://cmake.org/cmake/help/latest/prop_tgt/HEADER_SETS.html#prop_tgt:HEADER_SETS)， `consumerOnlyHeaders` 到 [`INTERFACE_HEADER_SETS`](https://cmake.org/cmake/help/latest/prop_tgt/INTERFACE_HEADER_SETS.html#prop_tgt:INTERFACE_HEADER_SETS)，以及 `publicHeaders` 将被添加到两者中。

当构建给定的目标时，它将使用自己的非接口属性（例如HEADER_SETS），并与它链接到的任何目标的接口属性相结合（例如interface_HEADER_SETS）。



**当从应用命令的目标的角度考虑命令时，范围关键字有一个简单的直觉：PRIVATE是给我的，INTERFACE是给其他人的，PUBLIC是给我们所有人的。**



### [`add_subdirectory()`](https://cmake.org/cmake/help/latest/command/add_subdirectory.html#command:add_subdirectory)

`add_subdirectory()` 命令允许我们将位于项目子目录中的 CML 文件纳入其中。

```c++
add_subdirectory(SubdirectoryName)
```

当`CMake`处理子目录中的`CMakeLists.txt`时，**子目录`CML`中描述的所有相对路径都是相对于该子目录的**，而不是顶级CML的。







# 第二节 CMake语言基础

在实际操作中，我们遇到的复杂性远不止简单描述源文件和头文件列表。为了应对这种复杂性，`CMake`提供了一种图灵完备的领域特定语言来描述构建软件的过程。当我们编写更复杂的`CML`和其他`CMake`文件时，了解这种语言的基本原理是必要的。这种语言的正式名称是"[`CMake Language`](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#manual:cmake-language(7))"，或者更通俗地说是`CMakeLang`。



*注意：`CMake` 语言并不适合描述与构建软件无关的内容。虽然它有一些通用功能，但开发者在解决与其构建语言无直接关系的问题时应谨慎。*

*通常，正确的答案是用通用编程语言编写一个工具来解决问题，并教`CMake`如何在构建过程中调用该工具。代码生成、加密签名工具，甚至射线跟踪器都是用`CMake`语言编写的，但这不是一种推荐的做法。*



## 背景

**`CMakeLang`中唯一的基本类型是字符串和列表。**`CMake`中的每个对象都是一个字符串，列表本身就是包含分号作为分隔符的字符串。任何看似对字符串以外的东西进行操作的命令，无论是布尔值、数字、JSON对象还是其他，实际上都在消耗字符串，执行一些内部转换逻辑（用`CMakeLang`以外的语言），然后转换回字符串以获得任何潜在的输出。



我们可以创建一个变量，也就是字符串的名称，使用 [`set（）`](https://cmake.org/cmake/help/latest/command/set.html#command:set) 命令。

```c++
set(var "World!")
```

变量的值可以使用大括号展开来访问，例如，如果我们想使用 [`message（）`](https://cmake.org/cmake/help/latest/command/message.html#command:message) 命令打印由`var`命名的字符串。

```c++
set(var "World!")
message("Hello ${var}")
```

```c++
$cmake -P CMakeLists.txt
Hello World!
```

*注意：[`cmake -P`](https://cmake.org/cmake/help/latest/manual/cmake.1.html#cmdoption-cmake-P) 被称为“脚本模式”，它通知`cmake`此文件不打算有 [`project（）`](https://cmake.org/cmake/help/latest/command/project.html#command:project)命令。我们没有构建任何软件，而是只使用`CMake`作为命令解释器。*



**因为`CMakeLang`只有字符串**，所以条件语句完全按照惯例，哪些字符串被认为是真的，哪些被认为是假的。这些应该是直观的，**“True”、“On”、“Yes”和（表示非零数字的字符串）都是真实的**，而**“False”、“Off”、“No”、“0”、“Ignore”、“NotFound”和空字符串都被认为是虚假的**。

然而，有些规则比这更复杂，因此花一些时间查阅关于表达式的 [`if（）`](https://cmake.org/cmake/help/latest/command/if.html#command:if) 文档是值得的。建议在给定的上下文中坚持使用一对，例如“True”/“False”或“On”/“Off”。



如前所述，列表是包含分号的字符串。[`list()`](https://cmake.org/cmake/help/latest/command/list.html#command:list)命令对于操作这些非常有用，`CMake`中的许多结构都希望使用这种约定。例如，我们可以使用[`foreach()`](https://cmake.org/cmake/help/latest/command/foreach.html#command:foreach)命令迭代列表。

```c++
set(stooges "Moe;Larry")
list(APPEND stooges "Curly")

message("Stooges contains: ${stooges}")

foreach(stooge IN LISTS stooges)
  message("Hello, ${stooge}")
endforeach()
```

```c++
$cmake -P CMakeLists.txt
Stooges contains: Moe;Larry;Curly
Hello, Moe
Hello, Larry
Hello, Curly
```



## 练习1-宏、函数和列表

CMake允许我们创建自己的函数和宏。在构建许多类似的目标（如测试）时，这可能非常有用，因为我们需要一遍又一遍地调用类似的命令集。我们用 [`function（）`](https://cmake.org/cmake/help/latest/command/function.html#command:function)和[`macro()`](https://cmake.org/cmake/help/latest/command/macro.html#command:macro)来实现。

```c++
macro(MyMacro MacroArgument)
  message("${MacroArgument}\n\t\tFrom Macro")
endmacro()

function(MyFunc FuncArgument)
  MyMacro("${FuncArgument}\n\tFrom Function")
endfunction()

MyFunc("From TopLevel")
```

```c++
$cmake -P CMakeLists.txt
From TopLevel
      From Function
              From Macro
```



与许多语言一样，函数和宏之间的区别在于范围。在`CMakeLang`中，[`function（）`](https://cmake.org/cmake/help/latest/command/function.html#command:function)和 [`macro（）`](https://cmake.org/cmake/help/latest/command/macro.html#command:macro) 都可以“看到”在它们上面的所有框架中创建的所有变量。然而， **[`macro（）`](https://cmake.org/cmake/help/latest/command/macro.html#command:macro) 在语义上类似于文本替换，类似于C/C++宏**，因此宏创建的任何副作用都可以在其调用上下文中看到。如果我们在宏中创建或更改变量，调用者将看到更改。

[`function（）`](https://cmake.org/cmake/help/latest/command/function.html#command:function) 创建自己的变量作用域，因此调用者看不到副作用。**为了向调用该函数的父函数传播变更，我们必须使用 `set（<var> <value> PARENT_SCOPE）`**， 其工作原理与 [`set（）`](https://cmake.org/cmake/help/latest/command/set.html#command:set) 相同，但适用于属于调用者上下文的变量。

*注意：在CMake 3.25中，添加了[`return(PROPAGATE)`](https://cmake.org/cmake/help/latest/command/return.html#command:return)选项，其工作原理与[`set(PARENT_SCOPE)`](https://cmake.org/cmake/help/latest/command/set.html#command:set) 相同，但提供了更好的人体工程学。*

[`macro()`](https://cmake.org/cmake/help/latest/command/macro.html#command:macro)和 [`function()`](https://cmake.org/cmake/help/latest/command/function.html#command:function) 都支持通过`ARGV`变量和`ARGN`变量的可变参数，**`ARGV`变量是一个包含传递给命令的所有参数的列表**，**`ARGN`变量包含最后一个预期参数之后的所有参数**。



## 练习2-条件句和循环

任何结构化编程语言中最常见的两个流控制元素是条件语句及其紧密的兄弟循环。CMakeLang也不例外。如前所述，给定CMake字符串的真实性是由`if（）`命令建立的约定。

当给定一个字符串时，`if（）`将**首先检查它是否是前面讨论的已知常数值之一**。如果字符串不是这些值之一，则命令假定它是一个变量，并检查该变量的大括号展开内容以确定条件的结果。

```c++
if(True)
  message("Constant Value: True")
else()
  message("Constant Value: False")
endif()

if(ConditionalValue)
  message("Undefined Variable: True")
else()
  message("Undefined Variable: False")
endif()

set(ConditionalValue True)

if(ConditionalValue)
  message("Defined Variable: True")
else()
  message("Defined Variable: False")
endif()
```

```c++
$cmake -P ConditionalValue.cmake
Constant Value: True
Undefined Variable: False
Defined Variable: True
```

*注意：这是讨论`CMake`中引用的好时机。`CMake`中的所有对象都是字符串，**因此双引号`"`，通常是不必要的**。`CMake`知道对象是字符串，一切都是字符串。*

*然而，在某些情况下，这是必要的。**包含空格的字符串需要双引号，否则它们将被视为列表**；`CMake`将用分号将元素连接在一起。反之亦然，当大括号展开列表时，如果我们想保留分号，就必须在引号内这样做。**否则`CMake`会将列表项展开为空格分隔的字符串。***

*一些命令，如 [`if()`](https://cmake.org/cmake/help/latest/command/if.html#command:if)，可以识别带引号和不带引号的字符串之间的区别。 [`if()`](https://cmake.org/cmake/help/latest/command/if.html#command:if)只会在字符串未加引号时检查给定字符串是否表示变量。*



最后，`if（）`提供了几种有用的比较模式，如`STREQUAL`用于字符串匹配，`DEFINED`用于检查变量的存在，`MATCHES`用于正则表达式检查。它还支持典型的逻辑运算符`NOT`、`AND`和`OR`。

除了条件之外，CMake还提供了两种循环结构， [`while()`](https://cmake.org/cmake/help/latest/command/while.html#command:while)，它遵循与[`if()`](https://cmake.org/cmake/help/latest/command/if.html#command:if)相同的规则来检查循环变量，以及更有用的[`foreach()`](https://cmake.org/cmake/help/latest/command/foreach.html#command:foreach)，其迭代字符串列表，并在[Background](https://cmake.org/cmake/help/latest/guide/tutorial/CMake Language Fundamentals.html#background)部分进行了演示。



```c++
function(FilterFoo OutVar)
# TODO3: Search all the variables in the argument list passed to FilterFoo,
#        and place those containing "Foo" into the list named by "OutVar"
  foreach(item IN LISTS ARGN)
	if(item MATCHES "Foo")
		list(APPEND ${OutVar} ${item})
	endif()
  endforeach()
  set(${OutVar} ${${OutVar}} PARENT_SCOPE)
endfunction()
```





# 第三节 配置和缓存变量

## 背景

如果我们有一个支持多种压缩算法的压缩软件`CMake`项目，我们可能想**让项目的打包者决定在构建我们的软件时启用哪些算法。我们可以通过使用通过[`-D`](https://cmake.org/cmake/help/latest/manual/cmake.1.html#cmdoption-cmake-D)标志设置的变量来实现。**

```c++
if(COMPRESSION_SOFTWARE_USE_ZLIB)
  message("I will use Zlib!")
  # ...
endif()

if(COMPRESSION_SOFTWARE_USE_ZSTD)
  message("I will use Zstd!")
  # ...
endif()
```

```c++
$cmake -B build \
    -DCOMPRESSION_SOFTWARE_USE_ZLIB=ON \
    -DCOMPRESSION_SOFTWARE_USE_ZSTD=OFF
...
I will use Zlib!
```



当然，我们希望**为这些配置选择提供合理的默认值，并提供一种传达给定选项目的的方法。此函数由[`option()`](https://cmake.org/cmake/help/latest/command/option.html#command:option)命令提供。**

```c++
option(COMPRESSION_SOFTWARE_USE_ZLIB "Support Zlib compression" ON)
option(COMPRESSION_SOFTWARE_USE_ZSTD "Support Zstd compression" ON)

if(COMPRESSION_SOFTWARE_USE_ZLIB)
  # Same as before
# ...
```

```c++
$cmake -B build \
    -DCOMPRESSION_SOFTWARE_USE_ZLIB=OFF
...
I will use Zstd!
```



由 [`-D`](https://cmake.org/cmake/help/latest/manual/cmake.1.html#cmdoption-cmake-D)标志和 [`option()`](https://cmake.org/cmake/help/latest/command/option.html#command:option)创建的名称不是普通变量，它们是**缓存变量**。缓存变量是全局可见的粘性变量，其值在初始设置后很难更改。事实上，它们非常**具有粘性**（我们可以在CMake配置运行之间编辑存储缓存变量的文件，即“CMake缓存”。这个文件是`build/configache.txt`，我们可以**在其中找到选项cache变量**。 *注意： **CMakeCache.txt 条目形式为 \<Name>：\<Type>=\<Value>，但“类型”只是提示。CMake 中的所有对象都是字符串，无论缓存如何。***），在项目模式下，CMake将跨多个配置保存和恢复缓存变量。**如果缓存变量设置了一次，它将一直保持到另一个 [`-D`](https://cmake.org/cmake/help/latest/manual/cmake.1.html#cmdoption-cmake-D) 标志抢占已保存的变量为止。**

*注意：CMake本身有几十个用于配置的普通变量和缓存变量。这些记录在 [`cmake-variables(7)`](https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html#manual:cmake-variables(7))中，其操作方式与项目提供的配置变量相同。*



**[`set()`](https://cmake.org/cmake/help/latest/command/set.html#command:set) 也可以用来操作缓存变量，但不会改变已经创建的变量。**

```c++
set(StickyCacheVariable "I will not change" CACHE STRING "")
set(StickyCacheVariable "Overwrite StickyCache" CACHE STRING "")

message("StickyCacheVariable: ${StickyCacheVariable}")
```

```c++
$cmake -P StickyCacheVariable.cmake
StickyCacheVariable: I will not change
```



**因为 [`-D`](https://cmake.org/cmake/help/latest/manual/cmake.1.html#cmdoption-cmake-D) 标志是在任何其他命令之前处理的，所以它们在设置缓存变量的值时具有优先权。**

```c++
$cmake \
  -DStickyCacheVariable="Commandline always wins" \
  -P StickyCacheVariable.cmake
StickyCacheVariable: Commandline always wins
```

 

**虽然缓存变量通常不能更改，但它们可以被普通变量遮挡**。我们可以通过[`set()`](https://cmake.org/cmake/help/latest/command/set.html#command:set)将一个变量设置为与缓存变量同名，然后使用[`unset()`](https://cmake.org/cmake/help/latest/command/unset.html#command:unset)删除普通变量来观察这一点。

```c++
set(ShadowVariable "In the shadows" CACHE STRING "")
set(ShadowVariable "Hiding the cache variable")
message("ShadowVariable: ${ShadowVariable}")

unset(ShadowVariable)
message("ShadowVariable: ${ShadowVariable}")
```

```c++
$cmake -P ShadowVariable.cmake
ShadowVariable: Hiding the cache variable
ShadowVariable: In the shadows
```



我们可以在命令行上更改缓存变量的值，因为**命令行在加载`CMakeCache.txt`之前运行，其值优先于缓存文件中的值**。

```c++
cmake -B build -DTUTORIAL_BUILD_UTILITIES=ON
cmake --build build
```

这样我们会观察到 `CMakeCache.txt` 的值从`OFF `状态翻转 到 `ON` 中，并且`Tutorial `可执行文件已经被构建完成。



## CMAKE变量

CMake提供了几个重要的正常和缓存变量，以允许打包者控制构建。编译器、默认标志、包的搜索位置等决策都由CMake自己的配置变量控制。

其中最重要的是语言标准。因为语言标准会对给定包所呈现的`ABI`产生重大影响。例如，库在后续标准中使用标准`C++`模板，并在早期标准中提供`polyfill`，这是很常见的。如果一个库在不同的标准下使用，那么标准模板和`polyfills`之间的`ABI`不兼容可能会导致无法理解的错误和运行时崩溃。

通过[`CMAKE_<LANG>_standard`](https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_STANDARD.html#variable:CMAKE__STANDARD)缓存变量，确保我们所有的目标都是在相同的语言标准下构建的。对于C++，这是`CMAKE_CXX_STANDARD`。

*注意：因为这些变量非常重要，所以开发人员不要在他们的CML中覆盖或隐藏它们同样重要。在CML中隐藏[`CMAKE_<LANG>_STANDARD`](https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_STANDARD.html#variable:CMAKE__STANDARD)，因为库需要C++20，当打包者决定用C++23构建其余的库和应用程序时，可能会导致上述可怕、难以理解的错误。*

*在没有充分理由的情况下，不要[`set()`](https://cmake.org/cmake/help/latest/command/set.html#command:set) `CMAKE_` globals。我们将在后面的步骤中讨论目标传达定义和最低标准等要求的更好方法。*



## CMakePresets.json

管理这些配置值可能会很快变得难以应对。在CI系统中，将这些记录为给定CI步骤的一部分是合适的。例如，在Github Actions CI步骤中，我们可能会看到类似于以下内容的内容：

```c++
- name: Configure and Build
  run: |
    cmake \
      -B build \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_CXX_STANDARD=20 \
      -DCMAKE_CXX_EXTENSIONS=ON \
      -DTUTORIAL_BUILD_UTILITIES=OFF \
      # Possibly many more options
      # ...

    cmake --build build
```

在本地开发代码时，即使只键入一次所有这些选项也可能容易出错。如果出于任何原因需要新的配置，多次这样做可能会让人筋疲力尽。

这个问题有许多不同的解决方案，您的选择最终取决于您作为开发人员的偏好。面向CLI的开发人员通常使用任务运行器来调用CMake，并为项目提供所需的选项。大多数IDE还具有用于控制CMake配置的自定义机制。

在这里不可能完全列举所有可能的配置工作流程。相反，我们将探索CMake的内置解决方案，即[`CMake Presets`](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html#manual:cmake-presets(7))。预设（ Presets）为我们提供了一种命名和表示CMake配置选项集合的格式。

*注意：预设能够表达整个CMake工作流程，从配置到构建，再到安装软件包。*

*他们比我们在这里能容纳的灵活得多。我们将仅限于使用它们进行配置。*

CMake预设包含两个标准文件：`CMakePresets.json`，该文件旨在作为项目的一部分并纳入版本控制；以及`CMakeUserPresets.json`，该文件用于本地用户配置，不应纳入版本控制。



对开发人员有用的最简单的预设只不过是配置变量。

```c++
{
  "version": 4,
  "configurePresets": [
    {
      "name": "example-preset",
      "cacheVariables": {
        "EXAMPLE_FOO": "Bar",
        "EXAMPLE_QUX": "Baz"
      }
    }
  ]
}
```



在调用CMake时，我们之前会这样做：

```c++
cmake -B build -DEXAMPLE_FOO=Bar -DEXAMPLE_QUX=Baz
```

我们现在可以使用预设：

```c++
cmake -B build --preset example-preset
```

CMake将搜索名为`CMakePresets.json`和`CMakeUserPresets.json`的文件，并从中加载命名的配置（如果可用）。

*注意：命令行标志可以与预设混合使用。命令行标志优先于预设中的值。*



预设还支持有限的宏，这些变量可以在预设内用大括号展开。我们唯一感兴趣的是`${sourceDir}`宏，它扩展到项目的根目录。我们可以使用它来设置我们的构建目录，在配置项目时跳过-B标志。

```c++
{
  "name": "example-preset",
  "binaryDir": "${sourceDir}/build"
}
```



# 第四节 深入CMake目标命令

CMake中有几个目标命令可用于描述需求。提醒一下，**目标命令是修改其应用到的目标属性的命令**。这些属性描述了构建软件所需的要求，如源代码、编译标志和输出名称；或使用目标所需的属性，如头包含、库目录和链接规则。

*注意：如`step1`所述，构建目标所需的属性应使用`PRIVATE`作用域关键字进行描述，使用`INTERFACE`使用目标所需属性应使用`PUBLIC`进行描述。*

在这一步中，我们将检查**CMake中所有可用的目标命令**。并非所有目标命令都是平等的。我们已经讨论了两个最重要的目标命令，`target_sources（）`和`target_link_libraies（）`。在剩下的命令中，有些几乎和这两个命令一样常见，另一些则有更高级的应用程序，只有在其他选项不可用时，才应将其中几个用作最后的手段。

## 背景

在继续之前，让我们命名所有CMake目标命令。我们将把这些命令分为三组：**推荐的和普遍有用的命令**、**高级的和警告性的命令**，以及**除非必要否则应该避免的`“footgun”`命令**。

|                      Common/Recommended                      |                       Advanced/Caution                       |                      Esoteric/Footguns                       |
| :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| [`target_compile_definitions()`](https://cmake.org/cmake/help/latest/command/target_compile_definitions.html#command:target_compile_definitions) | [`get_target_property()`](https://cmake.org/cmake/help/latest/command/get_target_property.html#command:get_target_property) | [`target_include_directories()`](https://cmake.org/cmake/help/latest/command/target_include_directories.html#command:target_include_directories) |
| [`target_compile_features()`](https://cmake.org/cmake/help/latest/command/target_compile_features.html#command:target_compile_features) | [`set_target_properties()`](https://cmake.org/cmake/help/latest/command/set_target_properties.html#command:set_target_properties) | [`target_link_directories()`](https://cmake.org/cmake/help/latest/command/target_link_directories.html#command:target_link_directories) |
| [`target_link_libraries()`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html#command:target_link_libraries) | [`target_compile_options()`](https://cmake.org/cmake/help/latest/command/target_compile_options.html#command:target_compile_options) |                                                              |
| [`target_sources()`](https://cmake.org/cmake/help/latest/command/target_sources.html#command:target_sources) | [`target_link_options()`](https://cmake.org/cmake/help/latest/command/target_link_options.html#command:target_link_options) |                                                              |
|                                                              | [`target_precompile_headers()`](https://cmake.org/cmake/help/latest/command/target_precompile_headers.html#command:target_precompile_headers) |                                                              |

*注意：没有所谓的`bad`（坏的）CMake目标命令。它们都有有效的用例。提供这种分类是为了让新手在解决问题时，对应该首先考虑哪些命令有一个简单的直觉。*



我们将在以下练习中演示其中的大部分。我们不会使用的三个是[`get_target_property()`](https://cmake.org/cmake/help/latest/command/get_target_property.html#command:get_target_property)、 [`set_target_properties()`](https://cmake.org/cmake/help/latest/command/set_target_properties.html#command:set_target_properties)和[`target_precompile_headers()`](https://cmake.org/cmake/help/latest/command/target_precompile_headers.html#command:target_precompile_headers)，因此我们将在这里简要讨论它们的用途。

[`get_target_property()`](https://cmake.org/cmake/help/latest/command/get_target_property.html#command:get_target_property)和[`set_target_properties()`](https://cmake.org/cmake/help/latest/command/set_target_properties.html#command:set_target_properties) 命令**可以按名称直接访问目标的属性**。它们甚至**可以用于将任意属性名称附加到目标。**

```c++
add_library(Example)
set_target_properties(Example
  PROPERTIES
    Key Value
    Hello World
)

get_target_property(KeyVar Example Key)
get_target_property(HelloVar Example Hello)

message("Key: ${KeyVar}")
message("Hello: ${HelloVar}")
```

```c++
$cmake -B build
...
Key: Value
Hello: World
```

[`cmake-properties(7)`](https://cmake.org/cmake/help/latest/manual/cmake-properties.7.html#manual:cmake-properties(7))中记录了对CMake具有语义意义的目标属性的完整列表，但其中大多数应该使用其专用命令进行修改。例如，没有必要直接操纵`LINK_LIBRARIES`和`INTERFACE_LINK_LIBRARIES`，因为它们是由[`target_link_libraries()`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html#command:target_link_libraries)处理的。

相反，一些较少使用的属性只能通过这些命令访问。[`DEPRECATION`](https://cmake.org/cmake/help/latest/prop_tgt/DEPRECATION.html#prop_tgt:DEPRECATION)属性用于将弃用通知附加到目标，只能通过[`set_target_properties()`](https://cmake.org/cmake/help/latest/command/set_target_properties.html#command:set_target_properties)设置；[`ADDITIONAL_CLEAN_FILES`](https://cmake.org/cmake/help/latest/prop_tgt/ADDITIONAL_CLEAN_FILES.html#prop_tgt:ADDITIONAL_CLEAN_FILES)也可以，用于描述CMake的干净目标要删除的其他文件；以及其他此类属性。

[`target_precompile_headers()`](https://cmake.org/cmake/help/latest/command/target_precompile_headers.html#command:target_precompile_headers) 命令获取一系列头文件，类似于[`target_sources()`](https://cmake.org/cmake/help/latest/command/target_sources.html#command:target_sources)，并从中创建一个预编译头文件。然后，这个预编译头被强制包含在目标中的所有翻译单元中。这对构建性能很有用。



## 特征与定义

在前面的步骤中，我们警告不要在全局内设置[`CMAKE_<LANG>_STANDARD`](https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_STANDARD.html#variable:CMAKE__STANDARD) ，也不要推翻打包商关于使用哪种语言标准的决定。另一方面，许多库都有构建所需的最小功能集，因此使用[`target_compile_features()`](https://cmake.org/cmake/help/latest/command/target_compile_features.html#command:target_compile_features)命令来传达这些要求是合适的。

```c++
target_compile_features(MyApp PRIVATE cxx_std_20)
```

[`target_compile_features()`](https://cmake.org/cmake/help/latest/command/target_compile_features.html#command:target_compile_features) 命令将最低语言标准描述为目标属性。如果[`CMAKE_<LANG>_STANDARD`](https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_STANDARD.html#variable:CMAKE__STANDARD) 高于此版本，或者编译器默认值已经提供了此语言标准，则不会采取任何行动。如果需要额外的标志来启用标准，这些标志将由CMake添加。

*注意：[`target_compile_features()`](https://cmake.org/cmake/help/latest/command/target_compile_features.html#command:target_compile_features)处理的接口和非接口属性样式与其他目标命令相同。这意味着可以继承用`INTERFACE`或`PUBLIC`范围关键字指定的语言标准要求。*

如果语言特性仅在实现文件中使用，那么相应的编译特性应该是`PRIVATE`。如果目标的标头使用了这些特性，则应使用`PUBLIC`或`INTERFACE`。



对于C++，编译特性的形式为`cxx_std_YY`，其中`YY`是标准化年，例如14、17、20等。

[`target_compile_definitions()`](https://cmake.org/cmake/help/latest/command/target_compile_definitions.html#command:target_compile_definitions)命令将编译定义描述为目标属性。它是将构建配置信息传递给源代码本身的最常见机制。正如我们所讨论的，与所有属性一样，作用域关键字也适用。

```c++
target_compile_definitions(MyLibrary
  PRIVATE
    MYLIBRARY_USE_EXPERIMENTAL_IMPLEMENTATION

  PUBLIC
    MYLIBRARY_EXCLUDE_DEPRECATED_FUNCTIONS
)
```

我们既不需要也不希望在编译用[`target_compile_definitions()`](https://cmake.org/cmake/help/latest/command/target_compile_definitions.html#command:target_compile_definitions)描述的定义时附加`-D`前缀。CMake将为当前编译器确定正确的标志。



## 编译和链接选项

有时，我们需要对编译和链接行上传递的确切选项进行特定控制。这些情况由 [`target_compile_options()`](https://cmake.org/cmake/help/latest/command/target_compile_options.html#command:target_compile_options) 和 [`target_link_options()`](https://cmake.org/cmake/help/latest/command/target_link_options.html#command:target_link_options)来解决。

```c++
target_compile_options(MyApp PRIVATE -Wall -Werror)
target_link_options(MyApp PRIVATE -T LinksScript.ld)
```

无条件调用[`target_compile_options()`](https://cmake.org/cmake/help/latest/command/target_compile_options.html#command:target_compile_options)或[`target_link_options()`](https://cmake.org/cmake/help/latest/command/target_link_options.html#command:target_link_options)有几个问题。主要问题是编译器标志特定于所使用的编译器前端。为了确保我们的项目支持多个编译器前端，我们必须只将兼容的标志传递给编译器。

我们可以通过检查 [`CMAKE_<LANG>_COMPILER_FRONTEND_VARIANT`](https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_FRONTEND_VARIANT.html#variable:CMAKE__COMPILER_FRONTEND_VARIANT)变量来实现这一点，该变量告诉我们编译器前端支持的标志的样式。

*注意：在CMake 3.26之前，CMake\_\<LANG>\_COMPILER_FRONTEND_VARIANT仅为具有多个前端变体的编译器设置。在CMake 3.26之后的版本中，仅检查此变量就足够了。*



## 包含和链接目录

如果我们碰巧有一些库或头文件不是由CMake目标描述的，我们需要将其带入构建中，也许是供应商提供的预编译二进制文件，我们可以将其与[`target_link_directories()`](https://cmake.org/cmake/help/latest/command/target_link_directories.html#command:target_link_directories) 和[`target_include_directories()`](https://cmake.org/cmake/help/latest/command/target_include_directories.html#command:target_include_directories)命令合并。

```c++
target_link_directories(MyApp PRIVATE Vendor/lib)
target_include_directories(MyApp PRIVATE Vendor/include)
```

这些命令使用映射到`-L`和`-I`编译器标志（或编译器用于链接和包含目录的任何标志）的属性。

当然，传递链接目录并不会告诉编译器将任何内容链接到构建中。为此，我们需要[`target_link_libraries()`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html#command:target_link_libraries)。当给[`target_link_libraries()`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html#command:target_link_libraries)一个不映射到目标名称的参数时，它会将字符串直接添加到链接行中，作为要链接到构建中的库（在前面添加任何适当的标志，如`-l`）。



# 第五节 深入解析 CMake 库概念

虽然可执行文件大多是一刀切的，但库有许多不同的形式。有静态存档、共享对象、模块、对象库、仅头库和描述其他目标继承的高级CMake属性的库，仅举几例。

在这一步中，您将了解CMake可以描述的一些最常见的库类型。这将涵盖[`add_library()`](https://cmake.org/cmake/help/latest/command/add_library.html#command:add_library)在项目中的大部分使用。从依赖项导入（或由项目导出以作为依赖项使用）的库将在后面的步骤中介绍。

## 背景

正如我们在step1中了解到的，[`add_library()`](https://cmake.org/cmake/help/latest/command/add_library.html#command:add_library)命令接受要创建的库目标的名称作为其第一个参数。第二个参数是可选的`<type>`，以下值对其有效：

- `STATIC`			[静态库 ](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#static-libraries)：链接其他目标时使用的对象文件的存档。
- `SHARED`            [共享库 ](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#shared-libraries)：一个动态库，可以由其他目标链接并在运行时加载。
- `MODULE`            [模块库 ](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#module-libraries)：一个插件，可能不会被其他目标链接，但可以在运行时使用类似dlopen的功能动态加载。
- `OBJECT`            [对象库 ](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#object-libraries)：尚未存档或链接到库中的对象文件的集合。
- `INTERFACE`      [接口库 ](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#interface-libraries)：一个库目标，指定依赖项的使用要求，但不编译源代码，也不在磁盘上生成库工件。

此外，还有`IMPORTED`库，用于描述从外部项目或模块导入到当前项目中的库目标。我们将在后面的步骤中简要介绍这些内容。

**`MODULE`库最常见于插件系统中，或作为`Python`或`Javascript`等运行时加载语言的扩展。**它们的行为与普通共享库非常相似，除了它们不能被其他目标直接链接。它们非常相似，我们在这里不再深入探讨。



## Static and Shared

虽然[`add_library()`](https://cmake.org/cmake/help/latest/command/add_library.html#command:add_library)命令支持显式设置STATIC或SHARED，这有时是必要的，但对于大多数可以作为其中之一操作的“普通”库，最好将第二个参数留空。

当没有给定类型时，[`add_library()`](https://cmake.org/cmake/help/latest/command/add_library.html#command:add_library) 将根据[`BUILD_SHARED_LIBS`](https://cmake.org/cmake/help/latest/variable/BUILD_SHARED_LIBS.html#variable:BUILD_SHARED_LIBS)的值创建STATIC或SHARED库。**如果[`BUILD_SHARED_LIBS`](https://cmake.org/cmake/help/latest/variable/BUILD_SHARED_LIBS.html#variable:BUILD_SHARED_LIBS)为真，将创建一个`SHARED`，否则它将是 `STATIC`。**

```c++
add_library(MyLib-static STATIC)
add_library(MyLib-shared SHARED)

# Depends on BUILD_SHARED_LIBS
add_library(MyLib)
```

这是一种理想的行为，因为它允许打包者确定将生成哪种库，并确保依赖项链接到该版本的库，而无需修改其源代码。在某些情况下，完全静态的构建是合适的，而在其他情况下，共享库是可取的。

***注意：CMake默认情况下不定义[`BUILD_SHARED_LIBS`](https://cmake.org/cmake/help/latest/variable/BUILD_SHARED_LIBS.html#variable:BUILD_SHARED_LIBS) 变量，这意味着在没有项目或用户干预的情况下， [`add_library()`](https://cmake.org/cmake/help/latest/command/add_library.html#command:add_library)将生成STATIC库。***

通过将[`add_library()`](https://cmake.org/cmake/help/latest/command/add_library.html#command:add_library)的第二个参数留空，项目为其打包者和下游依赖者提供了额外的灵活性。



## Interface Libraries

接口库是那些只传达其他目标的使用要求的库，它们不构建或生成任何自己的工件。因此，接口库的所有属性本身都必须是接口属性，用 `INTERFACE` [scope keywords](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#target-command-scope)（范围关键字）指定。

```c++
add_library(MyInterface INTERFACE)
target_compile_definitions(MyInterface INTERFACE MYINTERFACE_COMPILE_DEF)
```

C++开发中最常见的接口库是仅头文件库。这样的库不会构建任何东西，只提供发现其标头所需的标志。



## Object Libraries

对象库有几个高级用途，但也有一些棘手的细微差别，在本教程的范围内很难完全列举出来。

```c++
add_library(MyObjects OBJECT)
```

对象库最明显的缺点是对象本身不能传递链接。如果对象库出现在目标的[`INTERFACE_LINK_LIBRARIES`](https://cmake.org/cmake/help/latest/prop_tgt/INTERFACE_LINK_LIBRARIES.html#prop_tgt:INTERFACE_LINK_LIBRARIES) 中，则链接该目标的依赖项将无法“看到”对象。在这种情况下，对象库将充当`INTERFACE`库。在一般情况下，对象库只适合通过 [`target_link_libraries()`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html#command:target_link_libraries)进行`PRIVATE`或`PUBLIC`消费。

对象库的一个常见用例是将多个库目标合并到一个存档或共享库对象中。即使在单个项目中，由于各种原因，库也可能被维护为不同的目标，例如属于组织内的不同团队。然而，可能希望将这些作为面向单个消费者的二进制文件分发。对象库使这成为可能。



# 第六节 深入系统反思

为了发现有关系统环境和工具链的信息，CMake通常会编译小型测试程序，以验证编译器标志、头文件和内置程序或其他语言构造的可用性。

在这一步中，我们将利用CMake在我们自己的项目代码中使用的相同测试程序机制。

## 背景

回到配置和构建系统最古老的日子，一个古老的技巧是通过编译一个使用该功能的小程序来验证某些功能的可用性。

CMake使得这在许多情况下都是不必要的。正如我们将在后面的步骤中解决的那样，如果CMake可以找到库依赖关系，我们可以依靠它拥有我们期望的所有设施（头文件、代码生成器、测试实用程序等）。相反，如果CMake找不到依赖项，那么尝试使用该依赖项几乎肯定会失败。

然而，还有其他类型的关于工具链的信息，CMake并不容易沟通。对于这些高级情况，我们可以编写自己的测试程序并编译命令来检查可用性。

CMake提供了简化这些检查的模块。这些记录在 [`cmake-modules(7)`](https://cmake.org/cmake/help/latest/manual/cmake-modules.7.html#manual:cmake-modules(7))中。任何以`Check`开头的模块都是一个系统自检模块，我们可以使用它来查询工具链和系统环境。一些值得注意的包括：

- CheckIncludeFiles				检查一个或多个C/C++头文件。
- CheckCompilerFlag              检查编译器是否支持给定的标志。
- CheckSourceCompiles        检查是否可以为给定语言构建源代码。
- CheckIPOSupported            检查编译器是否支持过程间优化（IPO/LTO）。



## Check Include File

一个快速简单的**检查是给定的头文件是否在某个平台上可用**，**CMake为此提供了`CheckIncludeFiles`**。这最适合系统和内部标头，它们可能不是由特定的包提供的，但预计在许多构建环境中都可用。

```c++
include(CheckIncludeFiles)
check_include_files(sys/socket.h HAVE_SYS_SOCKET_H LANGUAGE CXX)
```

*注意：**这些函数在CMake中不能立即使用，必须通过其关联模块（又名CMakeLang文件）中的[`include()`](https://cmake.org/cmake/help/latest/command/include.html#command:include)添加。**许多模块位于CMake自己的`modules`文件夹中。这个内置的`Modules`文件夹是CMake在评估[`include()`](https://cmake.org/cmake/help/latest/command/include.html#command:include)命令时搜索的位置之一。您可以将这些模块视为标准库头文件，它们应该是可用的。*

一旦知道存在头文件，我们就可以使用已经涵盖的条件句和目标命令的相同机制将其传达给我们的代码。



## Check Source Compiles

有时，仅仅检查标题是不够的。当没有可供检查的标头时尤其如此，编译器内置就是这种情况。对于这些场景，我们有 [`CheckSourceCompiles`](https://cmake.org/cmake/help/latest/module/CheckSourceCompiles.html#module:CheckSourceCompiles)。

```c++
include(CheckSourceCompiles)
check_source_compiles(CXX
  "
    int main() {
      int a, b, c;
      __builtin_add_overflow(a, b, &c);
    }
  "
  HAS_CHECKED_ADDITION
)
```

*注意：默认情况下，[`CheckSourceCompiles`](https://cmake.org/cmake/help/latest/module/CheckSourceCompiles.html#module:CheckSourceCompiles)  会生成并链接可执行文件。要检查的代码必须提供有效的`int main()`才能成功。*

执行检查后，此系统自检可以与我们讨论头文件的方式相同地应用。

*注意：如果检查失败，而您认为应该成功，则需要通过删除`CMakeCache.txt`文件来清除`CMake缓存`。如果CMake有缓存结果，它将不会在后续运行中重新运行编译检查。*



## Check Interprocedural Optimization

过程间和链接时间优化可以为某些软件提供显著的性能改进。CMake能够通过[`CheckIPOSupported`](https://cmake.org/cmake/help/latest/module/CheckIPOSupported.html#module:CheckIPOSupported)检查IPO标志的可用性。

```c++
include(CheckIPOSupported)
check_ipo_supported() # fatal error if IPO is not supported
set_target_properties(MyApp
  PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION TRUE
)
```

注意：关于项目内IPO配置，有几个重要的注意事项：

- CMake并不知道每个编译器上的每个IPO/LTO标志，对已知的工具链进行单独调优通常可以获得更好的结果。
- 在目标上设置[`INTERPROCEDURAL_OPTIMIZATION`](https://cmake.org/cmake/help/latest/prop_tgt/INTERPROCEDURAL_OPTIMIZATION.html#prop_tgt:INTERPROCEDURAL_OPTIMIZATION)属性不会改变它链接到的任何目标或与其他项目的依赖关系。IPO只能“看到”其他目标，这些目标也得到了适当的编制。

**出于这些原因，应认真考虑通过外部机制（预设、-D标志、工具链文件等）而不是在项目控制中手动设置依赖树中所有项目的IPO/LTO标志。**

然而，特别是对于非常大的项目，有一个项目内机制在IPO可用时使用IPO可能是有用的。



# 第七节 自定义命令与生成文件

代码生成是一种普遍存在的机制，用于突破编程语言模型的限制来扩展其功能。CMake对Qt的元对象编译器提供了第一级支持，但其他代码生成器中鲜有能获得如此重视的。

相反，代码生成器往往是定制的，特定于使用情况。CMake提供了描述代码生成器使用的工具，因此项目可以添加对其个人需求的支持。

在这一步中，我们将使用[`add_custom_command()`](https://cmake.org/cmake/help/latest/command/add_custom_command.html#command:add_custom_command) 在教程项目中添加对代码生成器的支持。



## 背景

构建过程中的任何步骤通常都可以根据其输入和输出进行描述。CMake假设代码生成器和其他自定义进程基于相同的原理运行。这样，代码生成器的行为与编译器、链接器和工具链的其他元素相同；当输入比输出新（或输出不存在）时，将运行用户指定的命令来更新输出。

*注意：此模型假设流程的输出在运行之前是已知的。**CMake缺乏描述代码生成器的能力，其中输出的名称和位置取决于输入的内容**。存在各种黑客将此功能填充到CMake中，但它们不在本教程的范围内。*



描述代码生成器（或任何自定义过程）通常分为两部分。首先，**输入和输出的描述独立于CMake目标模型，只涉及生成过程本身**。其次，**输出与CMake目标相关联，以将其插入CMake目标模型中**。

对于源代码，这就像将生成的文件添加到`STATIC`、`SHARED`或`OBJECT`库的源代码列表中一样简单。对于仅包含头文件的生成器，通常需要使用通过 [`add_custom_target()`](https://cmake.org/cmake/help/latest/command/add_custom_target.html#command:add_custom_target)创建的中间目标将头文件生成添加到构建阶段（因为INTERFACE库没有构建步骤）。



## 使用代码生成器

描述代码生成器的主要机制是[`add_custom_command()`](https://cmake.org/cmake/help/latest/command/add_custom_command.html#command:add_custom_command) 命令。为了[`add_custom_command()`](https://cmake.org/cmake/help/latest/command/add_custom_command.html#command:add_custom_command) 的目的，“`command`”要么是构建环境中可用的可执行文件，要么是CMake可执行目标名称。

```c++
add_executable(Tool)
# ...
add_custom_command(
  OUTPUT Generated.cxx
  COMMAND Tool -i input.txt -o Generated.cxx
  DEPENDS Tool input.txt
  VERBATIM
)
# ...
add_library(GeneratedObject OBJECT)
target_sources(GeneratedObject
  PRIVATE
    Generated.cxx
)
```

除`VERBATIM`外，大多数关键字都是不言自明的。由于传统原因，这一论点实际上是强制性的，在现代背景下无法解释。好奇的人应该参考[`add_custom_command()`](https://cmake.org/cmake/help/latest/command/add_custom_command.html#command:add_custom_command) 文档以获取更多详细信息。

`tool`可执行目标同时出现在`COMMAND`和`DEPENDS`参数中。虽然`COMMAND`足以使代码正确构建，但将`tool`本身添加为自定义命令的依赖项可以确保如果工具更新，则会重新运行自定义命令。

对于仅生成头文件，需要额外的命令，因为库本身没有构建步骤。我们可以使用 [`add_custom_target()`](https://cmake.org/cmake/help/latest/command/add_custom_target.html#command:add_custom_target)为库创建一个“人工”构建步骤。然后，我们使用命令 [`add_dependencies()`](https://cmake.org/cmake/help/latest/command/add_dependencies.html#command:add_dependencies)强制在链接库的任何目标之前运行自定义目标。

```c++
add_custom_target(RunGenerator DEPENDS Generated.h)

add_library(GeneratedLib INTERFACE)
target_sources(GeneratedLib
  INTERFACE
    FILE_SET HEADERS
    BASE_DIRS
      ${CMAKE_CURRENT_BINARY_DIR}
    FILES
      ${CMAKE_CURRENT_BINARY_DIR}/Generated.h
)

add_dependencies(GeneratedLib RunGenerator)
```

*注意：我们将[`CMAKE_CURRENT_BINARY_DIR`](https://cmake.org/cmake/help/latest/variable/CMAKE_CURRENT_BINARY_DIR.html#variable:CMAKE_CURRENT_BINARY_DIR)（一个变量，用于命名构建树中放置工件的当前位置）添加到基目录中，因为这是我们的代码生成器将在其中运行的工作目录。列出文件对于构建来说是不必要的，在这里这样做只是为了清楚起见。*



# 第八节 测试和CTest

从历史上看，测试不是构建系统的作用。充其量，它可能有一个特定的目标，映射到构建和运行项目的测试。

在CMake生态系统中，情况正好相反。CMake的测试生态系统被称为CTest。这个生态系统既简单得令人难以置信，又强大得令人难以置信。事实上，它非常强大，值得有一个完整的教程来描述我们可以用它实现的一切。

这不是那个教程。在这一步中，我们将初步了解CTest提供的一些设施。



## 背景

CTest的核心是一个任务启动器，它在命令和报告返回零值或非零值时运行命令和报告。这是我们将与CTest打交道的级别。

CMake通过[`enable_testing()`](https://cmake.org/cmake/help/latest/command/enable_testing.html#command:enable_testing)和[`add_test()`](https://cmake.org/cmake/help/latest/command/add_test.html#command:add_test) 命令提供与CTest的直接集成。这些允许CMake在构建文件夹中设置必要的基础设施，以便CTest发现、运行和报告我们可能感兴趣的各种测试。	

设置和构建测试后，调用CTest的最简单方法是直接在构建目录上运行它，使用：

```c++
ctest --test-dir build
```

它将运行所有可用的测试。可以使用正则表达式运行特定的测试。

```c++
ctest --test-dir build -R SpecificTest
```

CTest还具有用于脚本编写、夹具、消毒器、作业服务器、指标报告等的高级机制。有关更多信息，请参阅[`ctest(1)`](https://cmake.org/cmake/help/latest/manual/ctest.1.html#manual:ctest(1))手册。

## 添加测试

CTest惯例规定，测试的构建和运行应基于名为[`BUILD_TESTING`](https://cmake.org/cmake/help/latest/variable/BUILD_TESTING.html#variable:BUILD_TESTING)的默认on变量。当通过[`CTest`](https://cmake.org/cmake/help/latest/module/CTest.html#module:CTest) 模块使用全套CTest功能时，此[`option()`](https://cmake.org/cmake/help/latest/command/option.html#command:option)是为我们设置的。当使用更精简的测试方法时，预计项目将自行设置该选项（或至少一个类似名称）。

当 [`BUILD_TESTING`](https://cmake.org/cmake/help/latest/variable/BUILD_TESTING.html#variable:BUILD_TESTING)为true时，应在根CML中调用[`enable_testing()`](https://cmake.org/cmake/help/latest/command/enable_testing.html#command:enable_testing)命令。

```c++
enable_testing()
```

这将在构建树中生成所有必要的元数据，以便CTest查找和运行测试。

一旦完成，就可以使用[`add_test()`](https://cmake.org/cmake/help/latest/command/add_test.html#command:add_test)命令在项目中的任何地方创建测试。此命令的语义类似于 [`add_custom_command()`](https://cmake.org/cmake/help/latest/command/add_custom_command.html#command:add_custom_command)；我们可以将可执行目标命名为“`command`”。

```c++
add_test(
  NAME MyAppWithTestFlag
  COMMAND MyApp --test
)
```



*注意：如果使用多配置生成器，例如`Visual Studio`，则需要使用`ctest-C<config><remaining flags>`指定配置，其中`<config>`是一个类似`Debug`或`Release`的值。无论何时使用多配置生成器，这都是正确的，在未来的命令中不会特别调用。*



# 第九节 安装命令和概念

项目需要做的不仅仅是构建和测试他们的代码，他们需要将其提供给消费者。构建树中的文件布局不适合其他项目使用，二进制文件位于意外的位置，头文件位于源代码树中的远处，并且没有明确的方法来发现提供了哪些目标或如何使用它们。

这种转换，将工件从源代码中移动出来，并将树构建成适合消费的最终布局，称为安装。CMake支持作为项目描述的一部分的完整安装工作流，控制安装树中工件的布局，并为希望使用安装树提供的库的其他CMake项目重建目标。



## 背景

所有CMake安装都要通过一个命令[`install()`](https://cmake.org/cmake/help/latest/command/install.html#command:install)，该命令被拆分为许多子命令，负责安装过程的各个方面。对于基于目标的CMake工作流，只需通过 [`install(TARGETS)`](https://cmake.org/cmake/help/latest/command/install.html#targets)安装目标本身就足够了，而无需通过[`install(FILES)`](https://cmake.org/cmake/help/latest/command/install.html#files) 或[`install(DIRECTORY)`](https://cmake.org/cmake/help/latest/command/install.html#directory).手动移动文件。

*注意：这就是为什么我们需要将FILES添加到要安装的头集。CMake需要能够在安装相关目标时定位文件。*

CMake将基于目标的安装分为各种工件类型。CMake 3.23中可用的工件类型有：
|                      ARCHIVE                      | 静态库（.a/.lib）、DLL导入库（.lib）和少数其他“类存档”对象。 |
| :-----------------------------------------------: | :----------------------------------------------------------: |
|                    **LIBRARY**                    | **共享库（.so）、模块和其他动态可加载对象。不是Windows的DLL文件（.DLL）或MacOS框架。** |
|                    **RUNTIME**                    | **除MacOS捆绑包之外的所有类型的可执行文件；以及Window的dll（.dll）。** |
|                    **OBJECT**                     |                    **OBJECT库中的对象。**                    |
|                   **FRAMEWORK**                   |                   **静态和共享MacOS框架**                    |
|                    **BUNDLE**                     |                  **MacOS捆绑包可执行文件**                   |
| **PUBLIC_HEADER` / `PRIVATE_HEADER` / `RESOURCE** | **由 [`PUBLIC_HEADER`](https://cmake.org/cmake/help/latest/prop_tgt/PUBLIC_HEADER.html#prop_tgt:PUBLIC_HEADER)、 [`PRIVATE_HEADER`](https://cmake.org/cmake/help/latest/prop_tgt/PRIVATE_HEADER.html#prop_tgt:PRIVATE_HEADER)和 [`RESOURCE`](https://cmake.org/cmake/help/latest/prop_tgt/RESOURCE.html#prop_tgt:RESOURCE)目标属性描述的文件，通常用于MacOS框架** |
|             **FILE_SET \<set-name>**              |      **与目标关联的文件集。这就是通常安装标头的方式。**      |



大多数重要的工件类型都有已知的目标，除非另有指示，否则CMake将默认使用这些目标。例如，如果变量可用，`RUNTIME`将安装到[`CMAKE_INSTALL_BINDIR`](https://cmake.org/cmake/help/latest/module/GNUInstallDirs.html#module:GNUInstallDirs)指定的位置，否则默认为`bin`。

下表描述了工件类型默认目标的完整列表:

|         Target Type         |           Variable            | Built-In Default |
| :-------------------------: | :---------------------------: | :--------------: |
|          `RUNTIME`          |   `${CMAKE_INSTALL_BINDIR}`   |      `bin`       |
|          `LIBRARY`          |   `${CMAKE_INSTALL_LIBDIR}`   |      `lib`       |
|          `ARCHIVE`          |   `${CMAKE_INSTALL_LIBDIR}`   |      `lib`       |
|      `PRIVATE_HEADER`       | `${CMAKE_INSTALL_INCLUDEDIR}` |    `include`     |
|       `PUBLIC_HEADER`       | `${CMAKE_INSTALL_INCLUDEDIR}` |    `include`     |
| `FILE_SET` (type `HEADERS`) | `${CMAKE_INSTALL_INCLUDEDIR}` |    `include`     |

在大多数情况下，项目应该保留默认值，除非需要安装到默认位置的特定子目录。

CMake默认情况下不定义`CMake_INSTALL_<dir>`变量。如果项目希望指定安装到这些位置之一的子目录中，则有必要包含[`GNUInstallDirs`](https://cmake.org/cmake/help/latest/module/GNUInstallDirs.html#module:GNUInstallDirs)模块，该模块将为所有尚未定义的`CMAKE_INSTALL_<dir>`变量提供值。



## 安装工件

对于现代的基于目标的CMake项目，工件的安装很简单，只需要一次调用[`install(targets)`](https://cmake.org/cmake/help/latest/command/install.html#targets)。

```c++
install(
  TARGETS MyApp MyLib

  FILE_SET HEADERS
  FILE_SET anotherHeaderFileSet
)
```

大多数工件类型都是默认安装的，不需要在[`install()`](https://cmake.org/cmake/help/latest/command/install.html#command:install)命令中列出。但是，必须命名`FILE_SET`才能让CMake知道您想安装。在上面的示例中，我们安装了两个文件集，一个名为`HEADERS`，另一个命名为`anotherHeaderFileSet`。

命名后，可以为工件类型提供各种选项，例如目的地。

```c++
include(GNUInstallDirs)

install(
  TARGETS MyApp MyLib

  RUNTIME
    DESTINATION ${CMAKE_INSTALL_BINDIR}/Subfolder

  FILE_SET HEADERS
)
```

这将把`MyApp`目标安装到`bin/Subfolder`（如果打包者没有更改[`CMAKE_INSTALL_BINDIR`](https://cmake.org/cmake/help/latest/module/GNUInstallDirs.html#module:GNUInstallDirs)）。

重要的是，如果从未给`OBJECT`工件类型指定目标，它将像`INTERFACE`库一样工作，只安装其头文件。



*注意：我们可以在定义目标的每个子文件夹中本地添加[`install(TARGETS)`](https://cmake.org/cmake/help/latest/command/install.html#targets)命令。这在很难跟踪所有可安装目标的大型项目中很常见。*



## 导出目标

这个安装文件的原始集合是一个良好的开端，但我们失去了CMake目标模型。这些实际上并不比我们在`Step 4`.中讨论的预编译的供应商库好。我们需要一些方法让其他项目根据我们在安装树中提供的内容重建我们的目标。

CMake提供的解决机制是一个名为“目标导出文件”的CMakeLang文件。它是由 [`install(EXPORT)`](https://cmake.org/cmake/help/latest/command/install.html#export)命令创建的。

```c++
install(
  TARGETS MyApp MyLib
  EXPORT MyProjectTargets
)

include(GNUInstallDirs)

install(
  EXPORT MyProjectTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MyProject
  NAMESPACE MyProject::
)
```

上面的例子有几个部分。首先，[`install(TARGETS)`](https://cmake.org/cmake/help/latest/command/install.html#targets)命令采用导出名称，基本上是一个用于添加已安装目标的列表。

稍后，[`install(EXPORT)`](https://cmake.org/cmake/help/latest/command/install.html#export)命令会使用此目标列表来生成目标导出文件。这将是一个名为`<ExportName>.cmake`的文件，位于提供的`DESTINATION`中。本例中提供的`DESTINATION`是常规的，但 [`find_package()`](https://cmake.org/cmake/help/latest/command/find_package.html#command:find_package)命令搜索的任何位置都是有效的。

最后，目标导出文件创建的目标将以`NAMESPACE`字符串作为前缀，即它们的格式为`<NAMESPACE><TargetName>`。通常，这是项目名称后跟两个冒号。

由于在未来步骤中会变得更加明显的原因，我们通常不会直接使用此文件。相反，我们有一个名为`<ProjectName>Config.cmake`的文件，可以通过[`include()`](https://cmake.org/cmake/help/latest/command/include.html#command:include)使用它。

```c++
include(${CMAKE_CURRENT_LIST_DIR}/MyProjectTargets.cmake)
```

[`CMAKE_CURRENT_LIST_DIR`](https://cmake.org/cmake/help/latest/variable/CMAKE_CURRENT_LIST_DIR.html#variable:CMAKE_CURRENT_LIST_DIR) 变量命名当前运行的CMAKE语言文件所在的目录，无论该文件是如何包含或启动的。

然后，此文件与目标导出一起[`install(FILES)`](https://cmake.org/cmake/help/latest/command/install.html#files)。

```c++
install(
  FILES
    cmake/MyProjectConfig.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MyProject
)
```

*注意：此文件的名称及其位置由 [`find_package()`](https://cmake.org/cmake/help/latest/command/find_package.html#command:find_package)命令的发现语义决定，我们将在下一步中对此进行更多讨论。*



*注意：与CTest一样，当使用多配置生成器时，例如Visual Studio，有必要使用`cmake --install --config <config> <remains flags>`指定配置，其中`<config>`是一个类似`Debug`或`Release`的值。无论何时使用多配置生成器，这都是正确的，在未来的命令中不会特别调用。*



## 导出版本文件

从目标导出文件导入CMake目标时，无法“退出”或“撤消”该操作。如果发现一个包是我们请求的版本的错误或不兼容版本，我们将在学习该版本信息时遇到任何副作用。

CMake为这个问题提供的答案是一个轻量级的版本文件，它只描述了这个版本兼容性信息，在CMake承诺完全导入文件之前可以检查这些信息。

CMake提供了用于生成这些版本文件的辅助模块和脚本，即[`CMakePackageConfigHelpers`](https://cmake.org/cmake/help/latest/module/CMakePackageConfigHelpers.html#module:CMakePackageConfigHelpers) 模块。

```c++
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/MyProjectConfigVersion.cmake
  COMPATIBILITY ExactVersion
)
```

可用版本有：

- AnyNewerVersion         	任何新版本
- SameMajorVersion           同大版本
- SameMinorVersion           同样小版本
- ExactVersion                      精确版本             

此外，包可以将自己标记为`ARCH_INDEPENDENT`，用于不附带二进制文件的包，这些二进制文件将它们与特定的机器架构联系起来。

默认情况下，`write_basic_package_VERSION_file（）`使用的版本是给定给[`project()`](https://cmake.org/cmake/help/latest/command/project.html#command:project)命令的版本号。





# 第十节 查找依赖关系

在C/C++软件开发中，管理构建依赖关系一直是现代开发人员面临的最高挑战之一。CMake提供了一个广泛的工具集，用于发现和验证不同类型的依赖关系。

但是，对于正确打包的项目，不需要使用这些高级工具。如今，许多流行的库和实用程序项目都会生成正确的安装树，就像我们在`Step 9`中设置的那样，很容易集成到CMake中。

在这种最佳情况下，我们只需要[`find_package()`](https://cmake.org/cmake/help/latest/command/find_package.html#command:find_package) 将依赖关系导入到我们的项目中。



## 背景

有五个主要命令用于发现CMake的依赖关系，前四个是：

| [`find_file()`](https://cmake.org/cmake/help/latest/command/find_file.html#command:find_file) |  查找并报告命名文件的完整路径，这往往是查找命令中最灵活的。  |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| **[`find_library()`](https://cmake.org/cmake/help/latest/command/find_library.html#command:find_library)** | **查找并报告适用于[`target_link_libraries()`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html#command:target_link_libraries)的静态存档或共享对象的完整路径。** |
| **[`find_path()`](https://cmake.org/cmake/help/latest/command/find_path.html#command:find_path)** | **查找并报告包含文件的目录的完整路径。这最常用于与[`target_include_directories()`](https://cmake.org/cmake/help/latest/command/target_include_directories.html#command:target_include_directories)结合使用的头文件。** |
| **[`find_program()`](https://cmake.org/cmake/help/latest/command/find_program.html#command:find_program)** | **查找并报告程序的可调用名称或路径。通常与[`execute_process()`](https://cmake.org/cmake/help/latest/command/execute_process.html#command:execute_process)或[`add_custom_command()`](https://cmake.org/cmake/help/latest/command/add_custom_command.html#command:add_custom_command)结合使用。** |

这些命令应被视为“备份”，在主查找命令不合适时使用。主要的find命令是[`find_package()`](https://cmake.org/cmake/help/latest/command/find_package.html#command:find_package)。它使用全面的内置启发式方法和上游提供的打包文件，为所请求的依赖关系提供最佳接口。



##  使用`find_package()`

 [`find_package()`](https://cmake.org/cmake/help/latest/command/find_package.html#command:find_package)使用的搜索路径和行为在其文档中有详细描述，但过于冗长，无法在此处复制。只需说，它搜索众所周知、鲜为人知、晦涩难懂和用户提供的位置，试图找到符合要求的包裹即可。

```c++
find_package(ForeignLibrary)
```

使用[`find_package()`](https://cmake.org/cmake/help/latest/command/find_package.html#command:find_package) 的最佳方法是确保在构建之前已将所有依赖项安装到单个安装树中，然后通过[`CMAKE_PREFIX_PATH`](https://cmake.org/cmake/help/latest/variable/CMAKE_PREFIX_PATH.html#variable:CMAKE_PREFIX_PATH)变量使[`find_package()`](https://cmake.org/cmake/help/latest/command/find_package.html#command:find_package) 知道该安装树的位置。

*注意：构建和安装依赖关系本身可能是一项巨大的劳动。虽然本教程将出于说明目的而这样做，但强烈建议使用包管理器进行项目本地依赖关系管理。*



[`find_package()`](https://cmake.org/cmake/help/latest/command/find_package.html#command:find_package) 接受除要查找的包之外的几个参数。最值得注意的是：

- **位置`<version>`参数，用于描述要根据包的配置版本文件检查的版本**。这应该谨慎使用，最好通过包管理器控制安装的依赖项的版本，而不是在其他无害的版本更新上破坏构建。如果已知该包依赖于依赖关系的旧版本，则使用版本要求可能是合适的。
- 非可选依赖项为`REQUIRED`，**如果找不到，则应中止构建**。
- `QUIET`用于可选依赖项，**当未找到时，不应向用户报告任何内容**。

[`find_package()`](https://cmake.org/cmake/help/latest/command/find_package.html#command:find_package)通过`<PackageName>_FOUND`变量报告其结果，这些变量**将分别为已找到和未找到的包设置为`true`或`false`值**。



## 传递性依赖

`Libraries`往往相互依存。多媒体应用程序可以依赖于提供对各种容器格式的支持的库，该库又可以依赖于用于压缩算法的一个或多个其他库。

我们需要在安装树中放置的包配置文件中表达这些可传递性要求。我们使用[`CMakeFindDependencyMacro`](https://cmake.org/cmake/help/latest/module/CMakeFindDependencyMacro.html#module:CMakeFindDependencyMacro)模块来实现这一点，该模块为已安装的包提供了一种递归发现彼此的安全机制。

```c++
include(CMakeFindDependencyMacro)
find_dependency(zlib)
```

[`find_dependency()`](https://cmake.org/cmake/help/latest/module/CMakeFindDependencyMacro.html#module:CMakeFindDependencyMacro)还转发来自顶级[`find_package()`](https://cmake.org/cmake/help/latest/command/find_package.html#command:find_package)调用的参数。如果[`find_package()`](https://cmake.org/cmake/help/latest/command/find_package.html#command:find_package)使用`QUIET`或`REQUIRED`调用，[`find_dependency()`](https://cmake.org/cmake/help/latest/module/CMakeFindDependencyMacro.html#module:CMakeFindDependencyMacro)也将使用`QUIET`和/或`REQUIRD`。



## 查找其他类型的文件

在一个完美的世界里，我们关心的每一个依赖项都会被正确打包，或者至少其他开发人员会编写一个模块来为我们发现它。我们并不生活在完美的世界中，有时我们不得不亲自动手，手动发现构建需求。

为此，我们在步骤前面枚举了其他find命令，例如[`find_path()`](https://cmake.org/cmake/help/latest/command/find_path.html#command:find_path)。

```c++
find_path(PackageIncludeFolder Package.h REQUIRED
  PATH_SUFFIXES
    Package
)
target_include_directories(MyApp
  PRIVATE
    ${PackageIncludeFolder}
)
```





# 第十一节 其他功能

有些功能不太合适，或者不够重要，无法在主教程中得到关注，但值得一提。这些练习收集了其中的一些特征。它们应该被视为“加分项”。

本教程没有涵盖许多CMake功能，其中一些功能被认为对使用它们的项目至关重要。其他版本通常由打包者使用，但在生成本地版本的软件开发人员中几乎没有讨论。

此列表并不是对CMake剩余功能的详尽讨论。它可能会随着时间和相关性而增长或萎缩。



## 目标别名

本教程侧重于安装依赖项并从安装树中使用它们。它还建议使用包管理器来促进这一过程。然而，由于历史和当代的各种原因，这并不总是CMake项目的使用方式。

可以在父项目中完全提供依赖项的源代码，并使用[`add_subdirectory()`](https://cmake.org/cmake/help/latest/command/add_subdirectory.html#command:add_subdirectory)使用它。执行此操作时，公开的目标名称是项目中使用的名称，而不是通过 [`install(EXPORT)`](https://cmake.org/cmake/help/latest/command/install.html#export)导出的名称。目标名称将没有命令前缀到目标的命名空间字符串。

一些项目希望通过与呈现给[`find_package()`](https://cmake.org/cmake/help/latest/command/find_package.html#command:find_package)消费者的接口一致的接口来支持此工作流。CMake通过 [`add_library(ALIAS)`](https://cmake.org/cmake/help/latest/command/add_library.html#alias)和[`add_executable(ALIAS)`](https://cmake.org/cmake/help/latest/command/add_executable.html#alias)支持这一点。

```c++
add_library(MyLib INTERFACE)
add_library(MyProject::MyLib ALIAS MyLib)
```



## 生成器表达式

[`Generator expressions`](https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#manual:cmake-generator-expressions(7))是CMake中某些上下文支持的复杂领域特定语言。它们最容易被理解为延迟评估条件，它们表达了在CMake配置阶段不知道确定正确行为的输入的需求。

*注意：这是生成器表达式命名的地方，在生成底层构建系统时对其进行求值。*

生成器表达式通常与[`target_include_directories()`](https://cmake.org/cmake/help/latest/command/target_include_directories.html#command:target_include_directories)）结合使用，以表达构建和安装树中的包含目录要求，但文件集已经取代了这种用例。它们现在最常见的应用是在多配置生成器和复杂的依赖注入系统中。

```c++
target_compile_definitions(MyApp PRIVATE "MYAPP_BUILD_CONFIG=$<CONFIG>")
```
