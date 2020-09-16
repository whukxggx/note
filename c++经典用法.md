# 非类型参数模板

关于模板：size可取具体数值，因为调用此函数时需要indest所需的内存大小，`if (!Ar.FastPathLoad<sizeof(Value)>(&Value))`,因此可以用这种。

疑问是为什么不直接做参数？

```c++
	template<SIZE_T Size>
	FORCEINLINE bool FastPathLoad(void* InDest)
	{
		const uint8* RESTRICT Src = ActiveFPLB->StartFastPathLoadBuffer;
		if (Src + Size <= ActiveFPLB->EndFastPathLoadBuffer)
		{
			if (Size == 2)
			{
				uint16 * RESTRICT Dest = (uint16 * RESTRICT)InDest;
				*Dest = FPlatformMemory::ReadUnaligned<uint16>(Src);
			}
			else if (Size == 4)
			{
				uint32 * RESTRICT Dest = (uint32 * RESTRICT)InDest;
				*Dest = FPlatformMemory::ReadUnaligned<uint32>(Src);
			}
			else if (Size == 8)
			{
				uint64 * RESTRICT Dest = (uint64 * RESTRICT)InDest;
				*Dest = FPlatformMemory::ReadUnaligned<uint64>(Src);
			}
			else
			{
				uint8 * RESTRICT Dest = (uint8 * RESTRICT)InDest;
				for (SIZE_T Index = 0; Index < Size; Index++)
				{
					Dest[Index] = Src[Index];
				}
			}
			ActiveFPLB->StartFastPathLoadBuffer += Size;
			return true;
		}
		return false;
	}
```

# 变长参数和变长参数模板以及std::initializer_list

# emplace_back和push_back

由于emplace_back的参数是一系列构建对象的参数，因此其可以直接构建，不需要临时对象。

这是非常有用的一个事，因为无论RVO(Return Value Optimization)和移动语义如何聪明，依然可能产生复杂的情况，push_back可能产生不必要的副本（或者移动）。

# RVO

RVO(Return Value Optimization)返回值优化是C++的一项编译优化技术。即删除保持函数返回值的临时对象。这可能会省略两次复制构造函数，即使构造函数有副作用。

# 移动语义

# RAII

```c++
class type_info
{
public:

    type_info(const type_info&) = delete;
    type_info& operator=(const type_info&) = delete;

    size_t hash_code() const noexcept
    {
        return __std_type_info_hash(&_Data);
    }

    bool operator==(const type_info& _Other) const noexcept
    {
        return __std_type_info_compare(&_Data, &_Other._Data) == 0;
    }

    bool operator!=(const type_info& _Other) const noexcept
    {
        return __std_type_info_compare(&_Data, &_Other._Data) != 0;
    }

    bool before(const type_info& _Other) const noexcept
    {
        return __std_type_info_compare(&_Data, &_Other._Data) < 0;
    }

    const char* name() const noexcept
    {
        #ifdef _M_CEE_PURE
        return __std_type_info_name(&_Data, static_cast<__type_info_node*>(__type_info_root_node.ToPointer()));
        #else
        return __std_type_info_name(&_Data, &__type_info_root_node);
        #endif
    }

    const char* raw_name() const noexcept
    {
        return _Data._DecoratedName;
    }

    virtual ~type_info() noexcept;

private:

    mutable __std_type_info_data _Data;
};
```

主要作用就是用于让用户知道是什么类型，提供比较方法，和name方法。

`const std::type_info& info=typeid(MyClass);`

# RTTI

# CRTP

# Thunk函数

# user-defined conversion function

[官网地址](https://en.cppreference.com/w/cpp/language/cast_operator)

此用法，可以让自定义的类型可以隐式或显式的转换到其他类型。具体操作如下:

`operator conversion-type-id`	(1)	
`explicit operator conversion-type-id`	(2)	(since C++11)
`explicit ( expression ) operator conversion-type-id`	(3)	(since C++20)

## 官方示例：

```c++
struct X {
    //implicit conversion
    operator int() const { return 7; }
 
    // explicit conversion
    explicit operator int*() const { return nullptr; }
 
//   Error: array operator not allowed in conversion-type-id
//   operator int(*)[3]() const { return nullptr; }
    using arr_t = int[3];
    operator arr_t*() const { return nullptr; } // OK if done through typedef
//  operator arr_t () const; // Error: conversion to array not allowed in any case
};
 
int main()
{
    X x;
 
    int n = static_cast<int>(x);   // OK: sets n to 7
    int m = x;                     // OK: sets m to 7
 
    int* p = static_cast<int*>(x);  // OK: sets p to null
//  int* q = x; // Error: no implicit conversion
 
    int (*pa)[3] = x;  // OK
}
```

注意，用户自定义的转换函数，不允许有返回类型。除了`explicit`之外，其他如`inline,virtual,constexpr,consteval,friend`等都可以使用。还需要注意的是不能转换为函数和数组，因此转换到一个指向数组的指针是需要重命名(using)或者用typedef或者用模板(template)。

## `nullptr`

定义大致如下，可以作为这方面参考的例子:

```c++
const class nullptr_t
{
public:
    //对于任何类指针返回0
    template<class T>
    inline operator T*() const
        { return 0; }

    //对于任何成员指针返回0
    template<class C, class T>
    inline operator T C::*() const
        { return 0; } 
private:
    void operator&() const;
} nullptr = {};
```

## 成员指针:

```cpp
#include <iostream>
using namespace std;

class Car
{
    public:
    int speed;
};

int main()
{
    int Car::*pSpeed = &Car::speed;

    Car c1;
    c1.speed = 1;       // direct access
    cout << "speed is " << c1.speed << endl;
    c1.*pSpeed = 2;     // access via pointer to member
    cout << "speed is " << c1.speed << endl;
    return 0;
}
```

这主要是提供了一种间接访问。