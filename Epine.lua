local tek = require "@nasso/epine-tek/v0.1.0-alpha8"

tek:project "rtk" {}

tek:binary "unit_tests" {
    language = "C++",
    srcs = { find "./tests/*.cpp"},
    libs = { "pthread", "criterion"},
    ldflags = {"--coverage"},
    cxxflags = {"--coverage -Wall -Wextra -pedantic"}

}

return tek:make()