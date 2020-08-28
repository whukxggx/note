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