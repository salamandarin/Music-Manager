add_library(Audio_Analyzer
        wav.cpp
)
target_compile_options(Audio_Analyzer PUBLIC -std=c++17)