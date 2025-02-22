target_sources(nnwcli PRIVATE
    parser/abstract_parser.cpp
    parser/argline_parser.cpp
    parser/placeholder_parser.cpp
    util/string_case.cpp
    util/utf8.cpp
    argument_types.cpp
    command.cpp
    command_executor.cpp
    context.cpp
)
target_sources(nnwcli_example PRIVATE
    main.cpp
)
