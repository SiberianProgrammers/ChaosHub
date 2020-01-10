import qbs

Project {
    references: [
        "libs/sqlite/sqlite.qbs",
        "libs/botan/botan.qbs",
    ]

    Application {
        files: [
            "README.md",
            "include/PointersTypedef.h",
            "include/Record.h",
            "include/RecordContent.h",
            "include/RecordsMaster.h",
            "include/RecordsModel.h",
            "include/Storage.h",
            "include/TextNote.h",
            "include/Tools.h",
            "source/Record.cpp",
            "source/RecordContent.cpp",
            "source/RecordsMaster.cpp",
            "source/RecordsModel.cpp",
            "source/Storage.cpp",
            "source/TextNote.cpp",
            "source/Tools.cpp",
            "source/main.cpp",
            "qml/qml.qrc",
            "translations/ChaosHub_ru_RU.ts",
        ]

        cpp.cxxLanguageVersion: "c++17"
        cpp.includePaths: ["include"]

        property pathList qmlImportPaths: [
            sourceDirectory + "/qml",
        ]

        Depends { name: "Qt"; submodules: ["core", "quick", "widgets"] }
        Depends { name: "sqlite" }
        Depends { name: "botan" }

        Group {
            fileTagsFilter: "application"
            qbs.install: true
            qbs.installDir: "bin"
        }
    }
}
