#include <iostream>

void calc()
{
    /* inputs in bytes*/
    unsigned input_data_size = 12;
    unsigned output_data_size = 2;
    unsigned cache_line_size = 64;

    /* output */
    unsigned batch_count = 1;

    do
    {
        if (
            (((input_data_size * batch_count) % cache_line_size) == 0) &&
            (((output_data_size * batch_count) % cache_line_size) == 0)
            )
        {
            break;
        }
        else
        {
            batch_count++;
        }

        if (batch_count > 10000)
        {
            std::cout << "fail" << std::endl;
            break;
        }
    } while (1);

    std::cout << "input_data_size =  " << input_data_size  << " bytes" << std::endl
              << "output_data_size = " << output_data_size << " bytes" << std::endl
              << "cache_line_size =  " << cache_line_size  << " bytes" << std::endl;
    std::cout << "----result---------" << std::endl;
    std::cout << "batch size = " << batch_count << std::endl
              << "input data size =  " << input_data_size * batch_count << " bytes" << std::endl
              << "output data size = " << output_data_size * batch_count << " bytes" << std::endl;
}

int main()
{
    calc();

    system("pause");
    return 0;
}
