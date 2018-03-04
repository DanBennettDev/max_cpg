#include "core.h"


#ifdef _DEBUG_LOG 
void dtb_text_to_log(const std::string &text)
{
    std::ofstream log_file(
        "log_file.txt", std::ios_base::out | std::ios_base::app);
    log_file << text << "\n";
    log_file.close();
}
#endif