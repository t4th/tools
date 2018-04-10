# batch calculator

Calculate number of buffers for optimal cache friendly data grouping.

## why

After watching Mike Acton video about data oriented programing i tested what he preached and this small tool is one of follow up prove of concept result.

## how

Just fill up input data information and run.

## example use case

Example code from directly from Mikes video: https://youtu.be/rX0ItVEVjHc?t=39m42s
```c++
struct FooUpdateIn {     // 12 bytes x count(32) = 384 = 64 x 6
  float m_Velocity[2];
  float m_Foo;
};

struct FooUpdateOut {    // 4 bytes x count(32) = 128 = 64 x 2
  float m_Foo;
};

void UpdateFoos(const FooUpdateIn * in, size_t count. FooUpdateOut * out, float f)
{
  for (size i = 0; i < count; i++) {
    float mag = sqrt(
      in[i].m_Velocity[0] * in[i].m_Velocity[0] +
      in[i].m_Velocity[1] * in[i].m_Velocity[1]);
      out[i].m_Foo = in[i].m_Foo + mag +f;
  }
}
```

Modify buffer and cache line size in bytes in main.cpp
```c++
    /* inputs in bytes*/
    unsigned input_data_size = 12;
    unsigned output_data_size = 4;
    unsigned cache_line_size = 64;
```

and this is the result:
```
input_data_size =  12 bytes
output_data_size = 4 bytes
cache_line_size =  64 bytes
----result---------
batch size = 16
input data size =  192 bytes
output data size = 64 bytes
Press any key to continue . . .
```

And now you can batch (group) buffers together in array of 16 (or multiply of 16 like case in video) to fill 100% cache lines.
