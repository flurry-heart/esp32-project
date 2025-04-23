# 源文件路径
set(src_dirs
    led
    key
    sht30
    lcd
    ultrasonic
    sdcard
    encoder
)

# 头文件路径
set(include_dirs
    led
    key
    sht30
    lcd
    ultrasonic
    sdcard
    encoder

    # ${CMAKE_SOURCE_DIR}表示顶级 CMakeLists.txt 文件所在的目录的绝对路径
    ${CMAKE_SOURCE_DIR}/components/peripheral/inc
    ${CMAKE_SOURCE_DIR}/components/toolkit
)

# 设置依赖库
set(requires
    driver
    fatfs
)

# 注册组件到构建系统的函数
idf_component_register(
    # 源文件路径
    SRC_DIRS ${src_dirs}
    # 自定义头文件的路径
    INCLUDE_DIRS ${include_dirs}
    # 依赖库的路径
    REQUIRES ${requires}
)

# 设置特定组件编译选项的函数
# -ffast-math: 允许编译器进行某些可能减少数学运算精度的优化，以提高性能。
# -O3: 这是一个优化级别选项，指示编译器尽可能地进行高级优化以生成更高效的代码。
# -Wno-error=format: 这将编译器关于格式字符串不匹配的警告从错误降级为警告。
# -Wno-format: 这将完全禁用关于格式字符串的警告。
component_compile_options(-ffast-math -O3 -Wno-error=format=-Wno-format)