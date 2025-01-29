// clang-format off
// NOLINTBEGIN(*-include-cleaner, *-avoid-magic-numbers, *-magic-numbers, *-unchecked-optional-access, *-avoid-do-while, *-use-anonymous-namespace, *-qualified-auto, *-suspicious-stringview-data-usage, *-err58-cpp, *-function-cognitive-complexity, *-macro-usage, *-unnecessary-copy-initialization)
// clang-format on

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_container_properties.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <future>

#include "testsConstanst.hpp"

using namespace std::literals::string_view_literals;

using Catch::Matchers::ContainsSubstring;
using Catch::Matchers::EndsWith;
using Catch::Matchers::Message;
using Catch::Matchers::MessageMatches;
using Catch::Matchers::StartsWith;
//  using Catch::Matchers::SizeIs;
//  using Catch::Matchers::Equals;

#define REQ_FORMAT(type, string) REQUIRE(FORMAT("{}", type) == (string));
#define REQ_FORMAT_COMPTOK(type, string) REQUIRE(FORMAT("{}", comp_tokType(type)) == (string));
#define MSG_FORMAT(...) Message(FORMAT(__VA_ARGS__))

TEST_CASE("extractTabs basic functionality", "[extractTabs]") {
    SECTION("String with only tabs") {
        const auto result = extractTabs(inputTabs);
        REQUIRE(result == inputTabs);  // Only tabs, so the result should be the same as the input
    }

    SECTION("String with tabs followed by other characters") {
        const auto result = extractTabs(inputMixed);
        REQUIRE(result == "\t\t\t");
    }

    SECTION("String without any tabs") {
        const auto result = extractTabs(inputText);
        REQUIRE(result.empty());  // No tabs, so the result should be empty
    }

    SECTION("String with no leading tabs but tabs in middle") {
        const auto result = extractTabs(inputMixedMidle);
        REQUIRE(result.empty());  // No leading tabs
    }

    SECTION("Empty string") {
        const std::string_view input;
        const std::string_view result = extractTabs(input);
        REQUIRE(result.empty());  // Empty input should return empty result
    }

    SECTION("String with mixed whitespace and tabs") {
        const auto result = extractTabs(inputMixedSpace);
        REQUIRE(result == "\t");
    }

    SECTION("String with multiple tabs and spaces") {
        const auto result = extractTabs(inputMixedSpace2);
        REQUIRE(result == "\t\t");  // Only leading tabs should be extracted, ignoring spaces
    }

    SECTION("String with only spaces") {
        const auto result = extractTabs(inputSpace);
        REQUIRE(result.empty());  // No tabs should result in an empty output
    }

    SECTION("String with a mix of leading tabs and spaces") {
        const auto result = extractTabs(inputMixedSpace3);
        REQUIRE(result == "\t");  // Only tabs until first non-tab character
    }
}

TEST_CASE("TimeValues initialization", "[TimeValues]") {
    using vnd::TimeValues;

    SECTION("Default Constructor") {
        const TimeValues time;
        REQUIRE(time.get_seconds() == 0.0L);
        REQUIRE(time.get_millis() == 0.0L);
        REQUIRE(time.get_micro() == 0.0L);
        REQUIRE(time.get_nano() == 0.0L);
    }

    SECTION("Initialization with nanoseconds") {
        const TimeValues time(1'000'000.0L);  // 1 millisecond in nanoseconds
        REQUIRE(time.get_seconds() == 0.001L);
        REQUIRE(time.get_millis() == 1.0L);
        REQUIRE(time.get_micro() == 1000.0L);
        REQUIRE(time.get_nano() == 1'000'000.0L);
    }

    SECTION("Initialization with individual time units") {
        const TimeValues time(1.0L, 1000.0L, 1'000'000.0L, 1'000'000'000.0L);  // 1 second
        REQUIRE(time.get_seconds() == 1.0L);
        REQUIRE(time.get_millis() == 1000.0L);
        REQUIRE(time.get_micro() == 1'000'000.0L);
        REQUIRE(time.get_nano() == 1'000'000'000.0L);
    }
}

TEST_CASE("ValueLabel functionality", "[ValueLabel]") {
    using vnd::ValueLabel;

    SECTION("Transform time in microseconds") {
        const ValueLabel value(1500.0L, "us");
        REQUIRE(value.transformTimeMicro(1500.0L) == "1500us,0ns");

        const ValueLabel valueNonExact(1500.5L, "us");
        REQUIRE(valueNonExact.transformTimeMicro(1500.5L) == "1500us,500ns");
    }

    SECTION("Transform time in milliseconds") {
        const ValueLabel value(2.5L, "ms");
        REQUIRE(value.transformTimeMilli(2.5L) == "2ms,500us,0ns");

        const ValueLabel valueNonExact(2.505L, "ms");
        REQUIRE(valueNonExact.transformTimeMilli(2.505L) == "2ms,505us,0ns");
    }

    SECTION("Transform time in seconds") {
        const ValueLabel value(1.0L, "s");
        REQUIRE(value.transformTimeSeconds(1.0L) == "1s,0ms,0us,0ns");

        const ValueLabel valueNonExact(1.005001L, "s");
        REQUIRE(valueNonExact.transformTimeSeconds(1.005001L) == "1s,5ms,1us,0ns");
    }

    SECTION("ToString based on time label") {
        const ValueLabel secondsVal(2.0L, "s");
        REQUIRE(secondsVal.toString() == "2s,0ms,0us,0ns");

        const ValueLabel millisVal(2500.0L, "ms");
        REQUIRE(millisVal.toString() == "2500ms,0us,0ns");

        const ValueLabel microsVal(1500.0L, "us");
        REQUIRE(microsVal.toString() == "1500us,0ns");

        const ValueLabel unknownVal(3.0L, "unknown");
        REQUIRE(unknownVal.toString() == "3 unknown");
    }
}

TEST_CASE("Times functionality for  nano seconds", "[Times]") {
    const vnd::Times time(10.0L);  // 1 millisecond
    REQUIRE(time.getRelevantTimeframe().toString() == "10 ns");
}

TEST_CASE("Times functionality", "[Times]") {
    using vnd::Times;
    using vnd::TimeValues;
    using vnd::ValueLabel;

    SECTION("Initialization with nanoseconds") {
        const Times time(1'000'000.0L);  // 1 millisecond
        const ValueLabel relevantTime = time.getRelevantTimeframe();
        REQUIRE(relevantTime.toString() == "1000us,0ns");
    }

    SECTION("Initialization with TimeValues and custom labels") {
        const TimeValues timeVals(0.5L, 500.0L, 500'000.0L, 500'000'000.0L);  // 0.5 seconds
        const Times time(timeVals, "seconds", "milliseconds", "microseconds", "nanoseconds");

        const ValueLabel relevantTime = time.getRelevantTimeframe();
        REQUIRE(relevantTime.toString() == "500 milliseconds");
    }

    SECTION("Switch between time units") {
        const TimeValues timeVals(0.001L, 1.0L, 1000.0L, 1'000'000.0L);  // 1 millisecond
        const Times time(timeVals);

        const ValueLabel relevantTime = time.getRelevantTimeframe();
        REQUIRE(relevantTime.toString() == "1000us,0ns");
    }

    SECTION("Very small nanoseconds") {
        const TimeValues timeVals(0.000001L, 0.001L, 1.0L, 1'000.0L);  // 1 microsecond
        const Times time(timeVals);

        const ValueLabel relevantTime = time.getRelevantTimeframe();
        REQUIRE(relevantTime.toString() == "1000 ns");
    }
}

TEST_CASE("Corner cases for TimeValues and Times", "[TimeValues][Times][CornerCases]") {
    using vnd::Times;
    using vnd::TimeValues;
    using vnd::ValueLabel;

    SECTION("Negative values") {
        const TimeValues negativeTime(-1000000.0L);  // -1 millisecond
        const Times time(negativeTime);

        const ValueLabel relevantTime = time.getRelevantTimeframe();
        REQUIRE(relevantTime.toString() == "-1000000 ns");
    }

    SECTION("Zero values") {
        const TimeValues zeroTime(0.0L);  // Zero nanoseconds
        const Times time(zeroTime);

        const ValueLabel relevantTime = time.getRelevantTimeframe();
        REQUIRE(relevantTime.toString() == "0 ns");
    }

    SECTION("Large values") {
        const long double largeValue = 1'000'000'000'000.0L;  // 1 second in nanoseconds
        const TimeValues largeTime(largeValue);               // 1 second
        const Times time(largeTime);

        const ValueLabel relevantTime = time.getRelevantTimeframe();
        REQUIRE(relevantTime.toString() == "1000s,0ms,0us,0ns");
    }
}

TEST_CASE("get_current_timestamp() tests", "[timestamp]") {
    SECTION("Basic test") {
        auto timestamp = get_current_timestamp();
        REQUIRE(timestamp.size() >= timestampSize);
    }

    SECTION("Repeatability test") {
        auto timestamp1 = get_current_timestamp();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        auto timestamp2 = get_current_timestamp();
        REQUIRE(timestamp1 != timestamp2);
    }

    SECTION("Concurrency test") {
        constexpr int num_threads = 4;
        std::vector<std::future<std::string>> futures;
        for(int i = 0; i < num_threads; ++i) {
            // NOLINTNEXTLINE(*-inefficient-vector-operation)
            futures.emplace_back(std::async(std::launch::async, []() { return get_current_timestamp(); }));
        }
        for(auto &future : futures) {
            auto timestamp = future.get();
            REQUIRE(timestamp.size() >= timestampSize);
        }
    }
}

TEST_CASE("createFile: Successfully create a file with content", "[FileCreationResult]") {
    const fs::path testDir = fs::temp_directory_path() / "test_file_creation";
    fs::create_directories(testDir);

    const std::string fileName = "test_file.txt";
    std::stringstream content;
    content << "Hello, this is a test file.";

    auto result = vnd::FileCreationResult::createFile(testDir, fileName, content);

    const fs::path createdFilePath = testDir / fileName;
    REQUIRE(result.success());
    REQUIRE(fs::exists(createdFilePath));

    const std::string filecontent = vnd::readFromFile(createdFilePath.string());

    REQUIRE(filecontent == content.str());

    // Cleanup
    fs::remove_all(testDir);
}

TEST_CASE("createFile: Attempt to create a file in a non-existent directory", "[FileCreationResult]") {
    const fs::path nonExistentDir = fs::temp_directory_path() / "non_existent_directory";
    const std::string fileName = "test_file.txt";
    std::stringstream content;
    content << "Content for non-existent directory test.";

    const auto result = vnd::FileCreationResult::createFile(nonExistentDir, fileName, content);

    REQUIRE_FALSE(result.success());
    REQUIRE(!fs::exists(nonExistentDir / fileName));
}

TEST_CASE("createFile: Handle file creation when file already exists", "[FileCreationResult]") {
    const fs::path testDir = fs::temp_directory_path() / "test_file_creation_existing";
    fs::create_directories(testDir);

    const std::string fileName = "existing_file.txt";
    std::stringstream initialContent;
    initialContent << "Initial content.";

    const fs::path existingFilePath = testDir / fileName;
    std::ofstream outfile(existingFilePath);
    outfile << initialContent.rdbuf();
    outfile.close();

    REQUIRE(fs::exists(existingFilePath));

    std::stringstream newContent;
    newContent << "New content that overwrites.";

    auto result = vnd::FileCreationResult::createFile(testDir, fileName, newContent);

    REQUIRE(result.success());
    REQUIRE(fs::exists(existingFilePath));

    const std::string filecontent = vnd::readFromFile(existingFilePath.string());

    REQUIRE(filecontent == newContent.str());

    // Cleanup
    fs::remove_all(testDir);
}

/*TEST_CASE("createFile: Handle exceptions during file creation", "[FileCreationResult]") {
    const fs::path invalidPath;
    std::string fileName = "invalid_file.txt";
    std::stringstream content;
    content << "This content should not be written.";

    auto result = vnd::FileCreationResult::createFile(invalidPath, fileName, content);

    REQUIRE_FALSE(result.success());
}*/

TEST_CASE("createFile: Attempt to create a file with empty content", "[FileCreationResult]") {
    const fs::path testDir = fs::temp_directory_path() / "test_empty_content";
    fs::create_directories(testDir);

    const std::string fileName = "empty_content_file.txt";
    const std::stringstream emptyContent;

    auto result = vnd::FileCreationResult::createFile(testDir, fileName, emptyContent);

    const fs::path createdFilePath = testDir / fileName;
    REQUIRE(result.success());
    REQUIRE(fs::exists(createdFilePath));

    const std::string filecontent = vnd::readFromFile(createdFilePath.string());

    REQUIRE(filecontent.empty());

    // Cleanup
    fs::remove_all(testDir);
}

static fs::path createTestFolderStructure() {
    fs::path testFolder = fs::temp_directory_path() / "test_folder_deletion";
    if(fs::exists(testFolder)) { fs::remove_all(testFolder); }

    fs::create_directories(testFolder / "subfolder1");
    fs::create_directories(testFolder / "subfolder2" / "nested");

    std::ofstream(testFolder / "file1.txt") << "File 1 content";
    std::ofstream(testFolder / "subfolder1" / "file2.txt") << "File 2 content";
    std::ofstream(testFolder / "subfolder2" / "nested" / "file3.txt") << "File 3 content";

    return testFolder;
}

TEST_CASE("deleteFile: Successfully delete an existing file", "[FileDelitionResult]") {
    const fs::path testFile = fs::temp_directory_path() / "test_file_to_delete.txt";

    // Create the test file
    std::ofstream(testFile) << "Sample content for deletion test";
    REQUIRE(fs::exists(testFile));

    const auto result = vnd::FileDelitionResult::deleteFile(testFile);

    REQUIRE(result.success());
    REQUIRE(!fs::exists(testFile));
}

TEST_CASE("deleteFile: Attempt to delete a non-existent file", "[FileDelitionResult]") {
    const fs::path nonExistentFile = fs::temp_directory_path() / "non_existent_file.txt";

    REQUIRE(!fs::exists(nonExistentFile));

    const auto result = vnd::FileDelitionResult::deleteFile(nonExistentFile);

    REQUIRE_FALSE(result.success());
}

TEST_CASE("deleteFile: Attempt to delete a directory instead of a file", "[FileDelitionResult]") {
    const fs::path testDirectory = fs::temp_directory_path() / "test_directory";
    fs::create_directories(testDirectory);

    REQUIRE(fs::exists(testDirectory));
    REQUIRE(fs::is_directory(testDirectory));

    const auto result = vnd::FileDelitionResult::deleteFile(testDirectory);

    REQUIRE_FALSE(result.success());
    REQUIRE(fs::exists(testDirectory));  // Ensure the directory is not accidentally deleted

    // Cleanup
    fs::remove_all(testDirectory);
}

/*TEST_CASE("deleteFile: Handle file with read-only permissions", "[FileDelitionResult]") {
    fs::path readOnlyFile = fs::temp_directory_path() / "read_only_file.txt";

    // Create the test file and set to read-only
    std::ofstream(readOnlyFile) << "Read-only file content";
    REQUIRE(fs::exists(readOnlyFile));

#ifdef _WIN32
    _chmod(readOnlyFile.string().c_str(), _S_IREAD);
#else
    chmod(readOnlyFile.string().c_str(), S_IRUSR);
#endif

    auto result = vnd::FileDelitionResult::deleteFile(readOnlyFile);

    REQUIRE(result.success());
    REQUIRE(!fs::exists(readOnlyFile));

    // No need to reset permissions since the file is deleted
}*/

TEST_CASE("deleteFile: Handle exceptions gracefully", "[FileDelitionResult]") {
    const fs::path invalidPath;

    const auto result = vnd::FileDelitionResult::deleteFile(invalidPath);

    REQUIRE_FALSE(result.success());
}

TEST_CASE("deleteFolder: Successfully delete an existing folder structure", "[FolderDeletionResult]") {
    const fs::path testFolder = createTestFolderStructure();
    REQUIRE(fs::exists(testFolder));

    const auto result = vnd::FolderDeletionResult::deleteFolder(testFolder);

    REQUIRE(result.success());
    REQUIRE(!fs::exists(testFolder));
}

TEST_CASE("deleteFolder: Attempt to delete a non-existent folder", "[FolderDeletionResult]") {
    const fs::path nonExistentFolder = fs::temp_directory_path() / "non_existent_folder";
    REQUIRE(!fs::exists(nonExistentFolder));

    const auto result = vnd::FolderDeletionResult::deleteFolder(nonExistentFolder);

    REQUIRE_FALSE(result.success());
}

TEST_CASE("deleteFolder: Attempt to delete a file path instead of a folder", "[FolderDeletionResult]") {
    const fs::path testFile = fs::temp_directory_path() / "test_file.txt";

    // Create the test file
    std::ofstream(testFile) << "Test content";
    REQUIRE(fs::exists(testFile));

    const auto result = vnd::FolderDeletionResult::deleteFolder(testFile);

    REQUIRE_FALSE(result.success());
    REQUIRE(fs::exists(testFile));  // Ensure the file is not accidentally deleted

    // Cleanup
    fs::remove(testFile);
}

/*TEST_CASE("deleteFolder: Handle folder with read-only file", "[FolderDeletionResult]") {
    fs::path testFolder = createTestFolderStructure();
    fs::path readOnlyFile = testFolder / "file1.txt";

    // Set the file to read-only using std::filesystem::permissions
    fs::permissions(readOnlyFile, fs::perms::owner_read, fs::perm_options::replace);

    auto result = vnd::FolderDeletionResult::deleteFolder(testFolder);

    REQUIRE_FALSE(result.success());

    // Restore file permissions and clean up
    fs::permissions(readOnlyFile, fs::perms::owner_all, fs::perm_options::replace);
    fs::remove_all(testFolder);
}*/

TEST_CASE("deleteFolder: Folder with nested subfolders and files", "[FolderDeletionResult]") {
    const fs::path testFolder = createTestFolderStructure();

    REQUIRE(fs::exists(testFolder));
    REQUIRE(fs::exists(testFolder / "subfolder1"));
    REQUIRE(fs::exists(testFolder / "subfolder2" / "nested" / "file3.txt"));

    auto result = vnd::FolderDeletionResult::deleteFolder(testFolder);

    REQUIRE(result.success());
    REQUIRE(!fs::exists(testFolder));
}

TEST_CASE("deleteFolder: Handle exceptions gracefully", "[FolderDeletionResult]") {
    const fs::path invalidPath;

    const auto result = vnd::FolderDeletionResult::deleteFolder(invalidPath);

    REQUIRE_FALSE(result.success());
}

TEST_CASE("singleCharOp function tests", "[singleCharOp]") {
    // Test valid operators
    REQUIRE(vnd::singoleCharOp('-') == vnd::TokenType::MINUS);
    REQUIRE(vnd::singoleCharOp('=') == vnd::TokenType::EQUAL);
    REQUIRE(vnd::singoleCharOp('<') == vnd::TokenType::LESS);
    REQUIRE(vnd::singoleCharOp('>') == vnd::TokenType::GREATER);
    REQUIRE(vnd::singoleCharOp('!') == vnd::TokenType::NOT);
    REQUIRE(vnd::singoleCharOp('+') == vnd::TokenType::PLUS);
    REQUIRE(vnd::singoleCharOp('*') == vnd::TokenType::STAR);
    REQUIRE(vnd::singoleCharOp('/') == vnd::TokenType::DIVIDE);
    REQUIRE(vnd::singoleCharOp('^') == vnd::TokenType::XOR);
    REQUIRE(vnd::singoleCharOp('%') == vnd::TokenType::PERCENT);
    REQUIRE(vnd::singoleCharOp('|') == vnd::TokenType::OR);
    REQUIRE(vnd::singoleCharOp('&') == vnd::TokenType::AND);

    // Test an unknown operator
    REQUIRE(vnd::singoleCharOp('a') == vnd::TokenType::UNKNOWN);
    REQUIRE(vnd::singoleCharOp(' ') == vnd::TokenType::UNKNOWN);  // Space character
    REQUIRE(vnd::singoleCharOp('#') == vnd::TokenType::UNKNOWN);  // Unrecognized symbol
}

TEST_CASE("my_error_handler(const std::string&) tests", "[error_handler]") {
    SECTION("Basic error handling") {
        const std::stringstream sss;
        auto *original = std::cerr.rdbuf(sss.rdbuf());  // Redirect cerr to stringstream
        my_error_handler("Sample error message");
        std::cerr.rdbuf(original);  // Restore cerr

        auto output = sss.str();
        REQUIRE_THAT(output, ContainsSubstring("Error occurred:"));
        REQUIRE_THAT(output, ContainsSubstring("Timestamp: "));
        REQUIRE_THAT(output, ContainsSubstring("Thread ID: "));
        REQUIRE_THAT(output, ContainsSubstring("Message: Sample error message"));
    }

    SECTION("Error handler with different messages") {
        const std::stringstream sss;
        auto *original = std::cerr.rdbuf(sss.rdbuf());  // Redirect cerr to stringstream
        my_error_handler("Error 1");
        my_error_handler("Another error");
        std::cerr.rdbuf(original);  // Restore cerr

        auto output = sss.str();
        REQUIRE_THAT(output, ContainsSubstring("Message: Error 1"));
        REQUIRE_THAT(output, ContainsSubstring("Message: Another error"));
    }

    SECTION("Location information") {
        const std::stringstream sss;
        auto *original = std::cerr.rdbuf(sss.rdbuf());  // Redirect cerr to stringstream
        my_error_handler("Location test");
        std::cerr.rdbuf(original);  // Restore cerr

        auto output = sss.str();
        REQUIRE_THAT(output, ContainsSubstring("File: "));
        REQUIRE_THAT(output, ContainsSubstring("Line: "));
        REQUIRE_THAT(output, ContainsSubstring("Column: "));
    }
}

TEST_CASE("std::filesystem::path formater", "[FMT]") { REQ_FORMAT(std::filesystem::path("../ssss"), "../ssss"); }

TEST_CASE("glm::vec formater", "[FMT]") {
    REQ_FORMAT(glm::vec2(0.0F, 0.0F), "vec2(0, 0)");
    REQ_FORMAT(glm::dvec2(0.0, 0.0), "dvec2(0, 0)");
    REQ_FORMAT(glm::ldvec2(0.0, 0.0), "ldvec2(0, 0)");
    REQ_FORMAT(glm::vec3(0.0F, 0.0F, 0.0F), "vec3(0, 0, 0)");
    REQ_FORMAT(glm::dvec3(0.0, 0.0, 0.0), "dvec3(0, 0, 0)");
    REQ_FORMAT(glm::ldvec3(0.0, 0.0, 0.0F), "ldvec3(0, 0, 0)");
    REQ_FORMAT(glm::vec4(0.0F, 0.0, 0.0, 0.0), "vec4(0, 0, 0, 0)");
    REQ_FORMAT(glm::dvec4(0.0, 0.0, 0.0, 0.0), "dvec4(0, 0, 0, 0)");
    REQ_FORMAT(glm::ldvec4(0.0, 0.0, 0.0F, 0.0), "ldvec4(0, 0, 0, 0)");
}

// clang-format off
TEST_CASE("glm::mat formater", "[FMT]") {
    REQ_FORMAT(glm::mat2(0.0F, 0.0F, 0.0F, 0.0F), "mat2x2((0,0), (0,0))");
    REQ_FORMAT(glm::dmat2(0.0, 0.0, 0.0, 0.0), "dmat2x2((0,0), (0,0))");
    REQ_FORMAT(glm::ldmat2(0.0, 0.0, 0.0, 0.0), "ldmat2x2((0,0), (0,0))");
    REQ_FORMAT(glm::mat3(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F), "mat3x3((0, 0, 0), (0, 0, 0), (0, 0, 0))");
    REQ_FORMAT(glm::dmat3(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0), "dmat3x3((0, 0, 0), (0, 0, 0), (0, 0, 0))");
    REQ_FORMAT(glm::ldmat3(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0), "ldmat3x3((0, 0, 0), (0, 0, 0), (0, 0, 0))");
    REQ_FORMAT(glm::mat4(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F), "mat4x4((0, 0, 0, 0), (0, 0, 0, 0), (0, 0, 0, 0), (0, 0, 0, 0))");
    REQ_FORMAT(glm::dmat4(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0), "dmat4x4((0, 0, 0, 0), (0, 0, 0, 0), (0, 0, 0, 0), (0, 0, 0, 0))");
    REQ_FORMAT(glm::ldmat4(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0), "ldmat4x4((0, 0, 0, 0), (0, 0, 0, 0), (0, 0, 0, 0), (0, 0, 0, 0))");
}


TEST_CASE("std::complex formater", "[FMT]") {
    REQ_FORMAT(std::complex<float>(1.0F, 2.0F), "(1, 2)");
    REQ_FORMAT(std::complex<double>(3.0, 4.0), "(3, 4)");
    REQ_FORMAT(std::complex<long double>(5.0L, 6.0L), "(5, 6)");
}

TEST_CASE("std::complex formater 2", "[FMT]") {
    REQ_FORMAT(std::complex<float>(1.1F, 2.1F), "(1.1, 2.1)");
    REQ_FORMAT(std::complex<double>(3.1, 4.1), "(3.1, 4.1)");
    REQ_FORMAT(std::complex<long double>(5.1L, 6.1L), "(5.1, 6.1)");
}

// clang-format on

TEST_CASE("Timer: MSTimes", "[timer]") {
    const auto timerNameData = timerName.data();
    vnd::Timer timer{timerNameData};
    std::this_thread::sleep_for(std::chrono::milliseconds(timerSleap));
    const std::string output = timer.to_string();
    const std::string new_output = (timer / timerCicles).to_string();
    REQUIRE_THAT(output, ContainsSubstring(timerNameData));
    REQUIRE_THAT(output, ContainsSubstring(timerNameData));
    REQUIRE_THAT(output, ContainsSubstring(timerTime1.data()));
    REQUIRE_THAT(new_output, ContainsSubstring(timerTime2.data()));
}

TEST_CASE("Timer: MSTimes FMT", "[timer]") {
    const auto timerNameData = timerName.data();
    vnd::Timer timer{timerNameData};
    std::this_thread::sleep_for(std::chrono::milliseconds(timerSleap));
    const std::string output = FORMAT("{}", timer);
    const std::string new_output = FORMAT("{}", (timer / timerCicles));
    REQUIRE_THAT(output, ContainsSubstring(timerNameData));
    REQUIRE_THAT(output, ContainsSubstring(timerTime1.data()));
    REQUIRE_THAT(new_output, ContainsSubstring(timerTime2.data()));
}

TEST_CASE("Timer: BigTimer", "[timer]") {
    const auto timerNameData = timerName.data();
    const vnd::Timer timer{timerNameData, vnd::Timer::Big};
    const std::string output = timer.to_string();
    REQUIRE_THAT(output, ContainsSubstring(timerNameData));
    REQUIRE_THAT(output, ContainsSubstring(timerBigs.data()));
}

TEST_CASE("Timer: BigTimer FMT", "[timer]") {
    const auto timerNameData = timerName.data();
    vnd::Timer timer{timerNameData, vnd::Timer::Big};
    const std::string output = FORMAT("{}", timer);
    REQUIRE_THAT(output, ContainsSubstring(timerNameData));
    REQUIRE_THAT(output, ContainsSubstring(timerBigs.data()));
}

TEST_CASE("Timer: AutoTimer", "[timer]") {
    const vnd::Timer timer;
    const std::string output = timer.to_string();
    REQUIRE_THAT(output, ContainsSubstring("Timer"));
}

TEST_CASE("Timer: PrintTimer", "[timer]") {
    std::stringstream out;
    const vnd::Timer timer;
    out << timer;
    const std::string output = out.str();
    REQUIRE_THAT(output, ContainsSubstring(timerName2.data()));
}

TEST_CASE("Timer: PrintTimer FMT", "[timer]") {
    vnd::Timer timer;
    const std::string output = FORMAT("{}", timer);
    REQUIRE_THAT(output, ContainsSubstring(timerName2.data()));
}

TEST_CASE("Timer: TimeItTimer", "[timer]") {
    vnd::Timer timer;
    const std::string output = timer.time_it([]() { std::this_thread::sleep_for(std::chrono::milliseconds(timerSleap2)); },
                                             timerResolution);
    REQUIRE_THAT(output, ContainsSubstring(timerTime1.data()));
}

TEST_CASE("CodeSourceLocation default constructor sets default values", "[CodeSourceLocation]") {
    const vnd::CodeSourceLocation location;
    REQUIRE(location.getFileName() == "unknown");
    REQUIRE(location.getLine() == 0);
    REQUIRE(location.getColumn() == 0);
}

TEST_CASE("CodeSourceLocation constructor sets values correctly", "[CodeSourceLocation]") {
    const vnd::CodeSourceLocation location(filename2, t_line2, t_colum6);
    REQUIRE(location.getFileName() == filename2);
    REQUIRE(location.getLine() == t_line2);
    REQUIRE(location.getColumn() == t_colum6);
}

TEST_CASE("CodeSourceLocation setters update values", "[CodeSourceLocation]") {
    vnd::CodeSourceLocation location;
    location.setFileName(filename3);
    location.setLine(t_line3);
    location.setColumn(t_colum6 * 2);
    REQUIRE(location.getFileName() == filename3);
    REQUIRE(location.getLine() == t_line3);
    REQUIRE(location.getColumn() == t_colum6 * 2);
}

TEST_CASE("CodeSourceLocation unknown() creates object with default values", "[CodeSourceLocation]") {
    const vnd::CodeSourceLocation location = vnd::CodeSourceLocation::unknown();
    REQUIRE(location.getFileName() == filename4);
    REQUIRE(location.getLine() == 0);
    REQUIRE(location.getColumn() == 0);
}

TEST_CASE("CodeSourceLocation equality and inequality operators work correctly", "[CodeSourceLocation]") {
    vnd::CodeSourceLocation location1(ffilename, t_line4, t_colum2);
    vnd::CodeSourceLocation location2(ffilename, t_line4, t_colum2);
    vnd::CodeSourceLocation location3("file2.cpp", t_line4, t_colum2);

    REQUIRE(location1 == location2);
    REQUIRE(location1 != location3);
}

TEST_CASE("CodeSourceLocation toString() produces expected string", "[CodeSourceLocation]") {
    const vnd::CodeSourceLocation location(filename2, t_line2, t_colum6);
    REQUIRE(location.toString() == "(file:example.cpp, line:42, column:10)");
}

TEST_CASE("corrected format for Tokentype", "[token_type]") {
    using enum vnd::TokenType;
    REQ_FORMAT(INTEGER, "INTEGER")
    REQ_FORMAT(DOUBLE, "DOUBLE")
    REQ_FORMAT(BOOLEAN, "BOOLEAN")
    REQ_FORMAT(PLUS, "PLUS_OPERATOR")
    REQ_FORMAT(MINUS, "MINUS_OPERATOR")
    REQ_FORMAT(EQUAL, "EQUAL_OPERATOR")
    REQ_FORMAT(DOT, "DOT_OPERATOR")
    REQ_FORMAT(STAR, "STAR_OPERATOR")
    REQ_FORMAT(DIVIDE, "DIVIDE_OPERATOR")
    REQ_FORMAT(XOR, "XOR_OPERATOR")
    REQ_FORMAT(PERCENT, "PERCENT_OPERATOR")
    REQ_FORMAT(OR, "OR_OPERATOR")
    REQ_FORMAT(AND, "AND_OPERATOR")
    REQ_FORMAT(LESS, "LESS_OPERATOR")
    REQ_FORMAT(GREATER, "GREATER_OPERATOR")
    REQ_FORMAT(PLUSPLUS, "PLUSPLUS_OPERATOR")
    REQ_FORMAT(MINUSMINUS, "MINUSMINUS_OPERATOR")
    REQ_FORMAT(PLUSEQUAL, "PLUSEQUAL_OPERATOR")
    REQ_FORMAT(MINUSEQUAL, "MINUSEQUAL_OPERATOR")
    REQ_FORMAT(NOTEQUAL, "NOTEQUAL_OPERATOR")
    REQ_FORMAT(STAREQUAL, "STAREQUAL_OPERATOR")
    REQ_FORMAT(DIVIDEEQUAL, "DIVIDEEQUAL_OPERATOR")
    REQ_FORMAT(XOREQUAL, "XOREQUAL_OPERATOR")
    REQ_FORMAT(PERCENTEQUAL, "PERCENTEQUAL_OPERATOR")
    REQ_FORMAT(OROR, "OROR_OPERATOR")
    REQ_FORMAT(ANDAND, "ANDAND_OPERATOR")
    REQ_FORMAT(EQUALEQUAL, "EQUALEQUAL_OPERATOR")
    REQ_FORMAT(LESSEQUAL, "LESSEQUAL_OPERATOR")
    REQ_FORMAT(GREATEREQUAL, "GREATEREQUAL_OPERATOR")
    REQ_FORMAT(IDENTIFIER, "IDENTIFIER")
    REQ_FORMAT(CHAR, "CHAR")
    REQ_FORMAT(STRING, "STRING")
    REQ_FORMAT(EOFT, "EOF")
    REQ_FORMAT(K_MAIN, "K_MAIN")
    REQ_FORMAT(K_VAR, "K_VAR")
    REQ_FORMAT(K_IF, "K_IF")
    REQ_FORMAT(K_WHILE, "K_WHILE")
    REQ_FORMAT(K_ELSE, "K_ELSE")
    REQ_FORMAT(K_FOR, "K_FOR")
    REQ_FORMAT(K_BREAK, "BREAK")
    REQ_FORMAT(K_FUN, "K_FUN")
    REQ_FORMAT(K_RETURN, "K_RETURN")
    REQ_FORMAT(K_NULLPTR, "K_NULLPTR")
    REQ_FORMAT(OPEN_PARENTESIS, "OPEN_PARENTESIS")
    REQ_FORMAT(OPEN_SQ_PARENTESIS, "OPEN_SQ_PARENTESIS")
    REQ_FORMAT(OPEN_CUR_PARENTESIS, "OPEN_CUR_PARENTESIS")
    REQ_FORMAT(CLOSE_PARENTESIS, "CLOSE_PARENTESIS")
    REQ_FORMAT(CLOSE_SQ_PARENTESIS, "CLOSE_SQ_PARENTESIS")
    REQ_FORMAT(CLOSE_CUR_PARENTESIS, "CLOSE_CUR_PARENTESIS")
    REQ_FORMAT(NOT, "NOT_OPERATOR")
    REQ_FORMAT(COMMA, "COMMA")
    REQ_FORMAT(COLON, "COLON")
    REQ_FORMAT(TYPE_I16, "TYPE_I16")
    REQ_FORMAT(TYPE_I32, "TYPE_I32")
    REQ_FORMAT(TYPE_I64, "TYPE_I64")
    REQ_FORMAT(TYPE_U8, "TYPE_U8")
    REQ_FORMAT(TYPE_U16, "TYPE_U16")
    REQ_FORMAT(TYPE_U32, "TYPE_U32")
    REQ_FORMAT(TYPE_U64, "TYPE_U64")
    REQ_FORMAT(TYPE_F32, "TYPE_F32")
    REQ_FORMAT(TYPE_F64, "TYPE_F64")
    REQ_FORMAT(TYPE_C32, "TYPE_C32")
    REQ_FORMAT(TYPE_C64, "TYPE_C64")
    REQ_FORMAT(TYPE_CHAR, "TYPE_CHAR")
    REQ_FORMAT(TYPE_STRING, "TYPE_STRING")
    REQ_FORMAT(TYPE_BOOL, "TYPE_BOOL")
    REQ_FORMAT(COMMENT, "COMMENT")
    REQ_FORMAT(UNKNOWN, "UNKNOWN")
}

TEST_CASE("corrected format for Tokentype compat to string", "[token_type]") {
    using enum vnd::TokenType;
    REQ_FORMAT_COMPTOK(INTEGER, "INT")
    REQ_FORMAT_COMPTOK(DOUBLE, "DBL")
    REQ_FORMAT_COMPTOK(BOOLEAN, "BOOL")
    REQ_FORMAT_COMPTOK(PLUS, "PLUS_OP")
    REQ_FORMAT_COMPTOK(MINUS, "MINUS_OP")
    REQ_FORMAT_COMPTOK(EQUAL, "EQUAL_OP")
    REQ_FORMAT_COMPTOK(DOT, "DOT_OP")
    REQ_FORMAT_COMPTOK(STAR, "STAR_OP")
    REQ_FORMAT_COMPTOK(DIVIDE, "DIVIDE_OP")
    REQ_FORMAT_COMPTOK(XOR, "XOR_OP")
    REQ_FORMAT_COMPTOK(PERCENT, "PERCENT_OP")
    REQ_FORMAT_COMPTOK(OR, "OR_OP")
    REQ_FORMAT_COMPTOK(AND, "AND_OP")
    REQ_FORMAT_COMPTOK(LESS, "LESS_OP")
    REQ_FORMAT_COMPTOK(GREATER, "GREATER_OP")
    REQ_FORMAT_COMPTOK(PLUSPLUS, "PLUSPLUS_OP")
    REQ_FORMAT_COMPTOK(MINUSMINUS, "MINUSMINUS_OP")
    REQ_FORMAT_COMPTOK(PLUSEQUAL, "PLUSEQUAL_OP")
    REQ_FORMAT_COMPTOK(MINUSEQUAL, "MINUSEQUAL_OP")
    REQ_FORMAT_COMPTOK(NOTEQUAL, "NOTEQUAL_OP")
    REQ_FORMAT_COMPTOK(STAREQUAL, "STAREQUAL_OP")
    REQ_FORMAT_COMPTOK(DIVIDEEQUAL, "DIVIDEEQUAL_OP")
    REQ_FORMAT_COMPTOK(XOREQUAL, "XOREQUAL_OP")
    REQ_FORMAT_COMPTOK(PERCENTEQUAL, "PERCENTEQUAL_OP")
    REQ_FORMAT_COMPTOK(OROR, "OROR_OP")
    REQ_FORMAT_COMPTOK(ANDAND, "ANDAND_OP")
    REQ_FORMAT_COMPTOK(EQUALEQUAL, "EQUALEQUAL_OP")
    REQ_FORMAT_COMPTOK(LESSEQUAL, "LESSEQUAL_OP")
    REQ_FORMAT_COMPTOK(GREATEREQUAL, "GREATEREQUAL_OP")
    REQ_FORMAT_COMPTOK(IDENTIFIER, "IDENT")
    REQ_FORMAT_COMPTOK(CHAR, "CH")
    REQ_FORMAT_COMPTOK(STRING, "STR")
    REQ_FORMAT_COMPTOK(EOFT, "EOF")
    REQ_FORMAT_COMPTOK(K_MAIN, "K_MAIN")
    REQ_FORMAT_COMPTOK(K_VAR, "K_VAR")
    REQ_FORMAT_COMPTOK(K_IF, "K_IF")
    REQ_FORMAT_COMPTOK(K_WHILE, "K_WHILE")
    REQ_FORMAT_COMPTOK(K_ELSE, "K_ELSE")
    REQ_FORMAT_COMPTOK(K_FOR, "K_FOR")
    REQ_FORMAT_COMPTOK(K_BREAK, "BREAK")
    REQ_FORMAT_COMPTOK(K_FUN, "K_FUN")
    REQ_FORMAT_COMPTOK(K_RETURN, "K_RETURN")
    REQ_FORMAT_COMPTOK(K_NULLPTR, "K_NULLPTR")
    REQ_FORMAT_COMPTOK(OPEN_PARENTESIS, "OPEN_PAR")
    REQ_FORMAT_COMPTOK(OPEN_SQ_PARENTESIS, "OPEN_SQ_PAR")
    REQ_FORMAT_COMPTOK(OPEN_CUR_PARENTESIS, "OPEN_CUR_PAR")
    REQ_FORMAT_COMPTOK(CLOSE_PARENTESIS, "CLOSE_PAR")
    REQ_FORMAT_COMPTOK(CLOSE_SQ_PARENTESIS, "CLOSE_SQ_PAR")
    REQ_FORMAT_COMPTOK(CLOSE_CUR_PARENTESIS, "CLOSE_CUR_PAR")
    REQ_FORMAT_COMPTOK(NOT, "NOT_OP")
    REQ_FORMAT_COMPTOK(COMMA, "COMMA")
    REQ_FORMAT_COMPTOK(COLON, "COLON")
    REQ_FORMAT_COMPTOK(TYPE_I8, "I8")
    REQ_FORMAT_COMPTOK(TYPE_I16, "I16")
    REQ_FORMAT_COMPTOK(TYPE_I32, "I32")
    REQ_FORMAT_COMPTOK(TYPE_I64, "I64")
    REQ_FORMAT_COMPTOK(TYPE_U8, "U8")
    REQ_FORMAT_COMPTOK(TYPE_U16, "U16")
    REQ_FORMAT_COMPTOK(TYPE_U32, "U32")
    REQ_FORMAT_COMPTOK(TYPE_U64, "U64")
    REQ_FORMAT_COMPTOK(TYPE_F32, "F32")
    REQ_FORMAT_COMPTOK(TYPE_F64, "F64")
    REQ_FORMAT_COMPTOK(TYPE_C32, "C32")
    REQ_FORMAT_COMPTOK(TYPE_C64, "C64")
    REQ_FORMAT_COMPTOK(TYPE_CHAR, "CHAR")
    REQ_FORMAT_COMPTOK(TYPE_STRING, "STRING")
    REQ_FORMAT_COMPTOK(TYPE_BOOL, "BOOL")
    REQ_FORMAT_COMPTOK(COMMENT, "COMMENT")
    REQ_FORMAT_COMPTOK(UNKNOWN, "UNKNOWN")
}

namespace {
    void modifyToken(vnd::Token &token) {
        token.setType(vnd::TokenType::INTEGER);
        token.setValue("assss");
        token.setFileName(filename);
        token.setLine(1);
        token.setColumn(1);
    }
    // Helper function to create a file with content
    // NOLINTBEGIN(*-easily-swappable-parameters, *-signed-bitwise)
    void createFile(const std::string &infilename, const std::string &content) {
        std::ofstream ofs(infilename, std::ios::out | std::ios::binary);
        ofs << content;
        ofs.close();
    }
    // NOLINTEND(*-easily-swappable-parameters, *-signed-bitwise)
}  // namespace

TEST_CASE("default constructed token", "[token]") {
    const vnd::Token token{};
    REQUIRE(token.getType() == vnd::TokenType::UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getFileName() == filename4);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
}

TEST_CASE("default constructed token toString", "[token]") {
    const vnd::Token token{};
    REQUIRE(token.getType() == vnd::TokenType::UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getFileName() == filename4);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
    REQUIRE(token.to_string() == "(type: UNKNOWN, value: '', source location:(file:unknown, line:0, column:0))");
}

TEST_CASE("default constructed token format", "[token]") {
    vnd::Token token{};
    REQUIRE(token.getType() == vnd::TokenType::UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getFileName() == filename4);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
    REQ_FORMAT(token, "(type: UNKNOWN, value: '', source location:(file:unknown, line:0, column:0))");
    REQUIRE(token.compat_to_string() == "(typ: UNKNOWN, val: '', sl:(f:unknown, l:0, c:0))");
}

TEST_CASE("default constructed token set propriety", "[token]") {
    using enum vnd::TokenType;
    vnd::Token token{};
    REQUIRE(token.getType() == UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getFileName() == filename4);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
    modifyToken(token);
    REQUIRE(token.getType() == INTEGER);
    REQUIRE(token.getValue().empty() == false);
    REQUIRE(token.getFileName() == filename);
    REQUIRE(token.getLine() == 1);
    REQUIRE(token.getColumn() == 1);
}

TEST_CASE("default constructed token isType", "[token]") {
    using enum vnd::TokenType;
    vnd::Token token{};
    REQUIRE(token.getType() == UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getFileName() == filename4);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
    REQUIRE(token.isType(CHAR) == false);
    modifyToken(token);
    REQUIRE(token.getType() == INTEGER);
    REQUIRE(token.isType(INTEGER) == true);
    REQUIRE(token.getValue().empty() == false);
    REQUIRE(token.getFileName() == filename);
    REQUIRE(token.getLine() == 1);
    REQUIRE(token.getColumn() == 1);
}

TEST_CASE("default constructed token set propriety tostring", "[token]") {
    using enum vnd::TokenType;
    vnd::Token token{};
    REQUIRE(token.getType() == UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
    REQUIRE(token.to_string() == "(type: UNKNOWN, value: '', source location:(file:unknown, line:0, column:0))");
    modifyToken(token);
    REQUIRE(token.getType() == INTEGER);
    REQUIRE(token.getValue().empty() == false);
    REQUIRE(token.getFileName() == filename);
    REQUIRE(token.getLine() == 1);
    REQUIRE(token.getColumn() == 1);
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    REQUIRE(token.to_string() == R"((type: INTEGER, value: 'assss', source location:(file:.\unknown.vn, line:1, column:1)))");
    REQUIRE(token.compat_to_string() == R"((typ: INT, val: 'assss', sl:(f:.\unknown.vn, l:1, c:1)))");
#else
    REQUIRE(token.to_string() == R"((type: INTEGER, value: 'assss', source location:(file:./unknown.vn, line:1, column:1)))");
    REQUIRE(token.compat_to_string() == R"((typ: INT, val: 'assss', sl:(f:./unknown.vn, l:1, c:1)))");
#endif
}

TEST_CASE("construct a empty value token", "[token]") {
    using enum vnd::TokenType;
    const vnd::Token token{UNKNOWN, vnd::CodeSourceLocation{}};
    REQUIRE(token.getType() == UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
    REQUIRE(token.getFileName() == "unknown");
    REQUIRE(token.to_string() == "(type: UNKNOWN, value: '', source location:(file:unknown, line:0, column:0))");
    REQUIRE(token.compat_to_string() == "(typ: UNKNOWN, val: '', sl:(f:unknown, l:0, c:0))");
}

TEST_CASE("construct a EOFT token", "[token]") {  // NOLINT(*-function-cognitive-complexity)
    using enum vnd::TokenType;
    const vnd::Token token{EOFT, vnd::CodeSourceLocation{}};
    REQUIRE(token.getType() == EOFT);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
    REQUIRE(token.getFileName() == "unknown");
    REQUIRE(token.to_string() == "(type: EOF, source location:(file:unknown, line:0, column:0))");
    REQUIRE(token.compat_to_string() == "(typ: EOF, sl:(f:unknown, l:0, c:0))");
}

TEST_CASE("FolderCreationResult Constructor", "[FolderCreationResult]") {
    SECTION("Default constructor") {
        const vnd::FolderCreationResult result;
        REQUIRE_FALSE(result.success());
        REQUIRE(result.path().value_or("").empty());
    }

    SECTION("Parameterized constructor") {
        const vnd::FolderCreationResult result(true, fs::path(testPaths));
        REQUIRE(result.success() == true);
        REQUIRE(result.path() == fs::path(testPaths));
    }
}

TEST_CASE("FolderCreationResult Setters", "[FolderCreationResult]") {
    vnd::FolderCreationResult result;

    SECTION("Set success") {
        result.set_success(true);
        REQUIRE(result.success() == true);
    }

    SECTION("Set path") {
        fs::path testPath(testPaths);
        REQUIRE(result.path().value_or("").empty());
        result.set_path(testPaths);
        REQUIRE(result.path() == testPath);
    }

    SECTION("Set path with empty string") {
        REQUIRE_THROWS_MATCHES(result.set_path(fs::path()), std::invalid_argument, Message("Path cannot be empty"));
    }
}

TEST_CASE("FolderCreationResult operator<< outputs correctly", "[FolderCreationResult]") {
    SECTION("Test with successful folder creation and valid path") {
        const fs::path folderPath = "/test/directory";
        const vnd::FolderCreationResult result(true, folderPath);

        std::ostringstream oss;
        oss << result;

        REQUIRE(oss.str() == "success_: true, path_: /test/directory");
    }

    SECTION("Test with unsuccessful folder creation and no path") {
        const vnd::FolderCreationResult result(false, fs::path{});

        std::ostringstream oss;
        oss << result;

        REQUIRE(oss.str() == "success_: false, path_: None");
    }

    SECTION("Test with successful folder creation but empty path") {
        const vnd::FolderCreationResult result(true, fs::path{});

        std::ostringstream oss;
        oss << result;

        REQUIRE(oss.str() == "success_: true, path_: None");
    }

    SECTION("Test with unsuccessful folder creation and valid path") {
        const fs::path folderPath = "/another/test/directory";
        const vnd::FolderCreationResult result(false, folderPath);

        std::ostringstream oss;
        oss << result;

        REQUIRE(oss.str() == "success_: false, path_: /another/test/directory");
    }

    SECTION("Test with default constructed FolderCreationResult") {
        const vnd::FolderCreationResult result;

        std::ostringstream oss;
        oss << result;

        REQUIRE(oss.str() == "success_: false, path_: None");
    }
}

TEST_CASE("FolderCreationResult: Equality and Swap", "[FolderCreationResult]") {
    fs::path path1("/folder1");
    fs::path path2("/folder2");

    vnd::FolderCreationResult result1(true, path1);
    vnd::FolderCreationResult result2(false, path2);

    SECTION("Equality operator") {
        REQUIRE(result1 != result2);
        vnd::FolderCreationResult result3(true, path1);
        REQUIRE(result1 == result3);
    }

    SECTION("swap() function") {
        swap(result1, result2);
        REQUIRE(result1.success() == false);
        REQUIRE(result1.path().value() == path2);
        REQUIRE(result2.success() == true);
        REQUIRE(result2.path().value() == path1);
    }
}

TEST_CASE("FolderCreationResult Hash Value", "[FolderCreationResult]") {
    SECTION("Hash value is consistent for the same object") {
        const vnd::FolderCreationResult result(true, fs::path("/test/directory"));
        const std::size_t hash1 = hash_value(result);
        const std::size_t hash2 = hash_value(result);

        REQUIRE(hash1 == hash2);
    }

    SECTION("Hash value changes with different success status") {
        const vnd::FolderCreationResult result1(true, fs::path("/test/directory"));
        const vnd::FolderCreationResult result2(false, fs::path("/test/directory"));

        const std::size_t hash1 = hash_value(result1);
        const std::size_t hash2 = hash_value(result2);

        REQUIRE(hash1 != hash2);
    }

    SECTION("Hash value changes with different paths") {
        const vnd::FolderCreationResult result1(true, fs::path("/test/directory"));
        const vnd::FolderCreationResult result2(true, fs::path("/different/directory"));

        const std::size_t hash1 = hash_value(result1);
        const std::size_t hash2 = hash_value(result2);

        REQUIRE(hash1 != hash2);
    }

    SECTION("Identical objects have the same hash value") {
        const vnd::FolderCreationResult result1(true, fs::path("/test/directory"));
        const vnd::FolderCreationResult result2(true, fs::path("/test/directory"));

        const std::size_t hash1 = hash_value(result1);
        const std::size_t hash2 = hash_value(result2);

        REQUIRE(hash1 == hash2);
    }

    SECTION("Different objects have different hash values") {
        const vnd::FolderCreationResult result1(true, fs::path("/test/directory"));
        const vnd::FolderCreationResult result2(false, fs::path("/another/directory"));

        const std::size_t hash1 = hash_value(result1);
        const std::size_t hash2 = hash_value(result2);

        REQUIRE(hash1 != hash2);
    }

    SECTION("Hash for default constructed object is consistent") {
        const vnd::FolderCreationResult result1;
        const vnd::FolderCreationResult result2;

        const std::size_t hash1 = hash_value(result1);
        const std::size_t hash2 = hash_value(result2);

        REQUIRE(hash1 == hash2);
    }

    SECTION("Hash for default object vs object with empty path") {
        const vnd::FolderCreationResult result1;
        const vnd::FolderCreationResult result2(false, fs::path{});

        const std::size_t hash1 = hash_value(result1);
        const std::size_t hash2 = hash_value(result2);

        REQUIRE(hash1 == hash2);
    }
}

TEST_CASE("FolderCreationResult Folder Creation Functions", "[FolderCreationResult]") {
    // Create a temporary directory for testing
    auto tempDir = fs::temp_directory_path() / "vnd_test";
    const std::string folderName = "test_folder";
    const fs::path folderPath = tempDir / folderName;
    fs::create_directories(tempDir);

    SECTION("Create folder with valid parameters") {
        const vnd::FolderCreationResult result = vnd::FolderCreationResult::createFolder(folderName, tempDir);
        REQUIRE(result.success() == true);
        REQUIRE(result.path() == folderPath);
        [[maybe_unused]] auto unused = fs::remove_all(folderPath);
    }

    SECTION("Create folder with empty folder name") {
        const std::string emptyFolderName;
        const vnd::FolderCreationResult result = vnd::FolderCreationResult::createFolder(emptyFolderName, tempDir);
        REQUIRE_FALSE(result.success());
        REQUIRE(result.path()->empty());
    }

    SECTION("Create folder in non-existent parent directory") {
        const fs::path nonExistentParentDir = tempDir / "non_existent_dir";
        const vnd::FolderCreationResult result = vnd::FolderCreationResult::createFolder(folderName, nonExistentParentDir);
        REQUIRE(result.success() == true);
        REQUIRE(!result.path()->empty());
    }

    SECTION("Create folder in existing directory") {
        const fs::path nonExistentParentDir = tempDir / "non_existent_dir";
        const vnd::FolderCreationResult result = vnd::FolderCreationResult::createFolder(folderName, nonExistentParentDir);
        REQUIRE(result.success() == true);
        REQUIRE(!result.path()->empty());
        const std::string folderName2 = "test_folder";
        const vnd::FolderCreationResult result2 = vnd::FolderCreationResult::createFolder(folderName2, nonExistentParentDir);
        REQUIRE(result2.success() == true);
        REQUIRE(!result2.path()->empty());
    }

    SECTION("Create folder next to non-existent file") {
        const fs::path nonExistentFilePath = tempDir / "non_existent_file.txt";
        const vnd::FolderCreationResult result = vnd::FolderCreationResult::createFolderNextToFile(nonExistentFilePath, folderName);
        REQUIRE(result.success() == true);
        REQUIRE(!result.path()->empty());
        REQUIRE(!result.pathcref()->empty());
    }

    SECTION("Create folder next to existing file") {
        // Create a file in the temporary directory
        const fs::path filePathInner = tempDir / "test_file.txt";
        std::ofstream ofs(filePathInner);
        ofs.close();

        const vnd::FolderCreationResult result = vnd::FolderCreationResult::createFolderNextToFile(filePathInner, folderName);
        REQUIRE(result.success() == true);
        REQUIRE(result.path() == folderPath);

        [[maybe_unused]] auto unused = fs::remove(filePathInner);
        [[maybe_unused]] auto unuseds = fs::remove_all(folderPath);
    }
    [[maybe_unused]] auto unused = fs::remove_all(tempDir);
}

TEST_CASE("vnd::readFromFile - Valid File", "[file]") {
    const std::string infilename = "testfile.txt";
    const std::string content = "This is a test.";

    createFile(infilename, content);

    SECTION("Read from valid file") {
        auto result = vnd::readFromFile(infilename);
        REQUIRE(result == content);  // Ensure the content matches
    }

    [[maybe_unused]] auto unsed = fs::remove(infilename);
}

TEST_CASE("vnd::readFromFile - Non-existent File", "[file]") {
    const std::string nonExistentFile = "nonexistent.txt";

    SECTION("Read from non-existent file") {
        REQUIRE_THROWS_MATCHES(vnd::readFromFile(nonExistentFile), std::runtime_error, MSG_FORMAT("File not found: {}", nonExistentFile));
    }
}

TEST_CASE("vnd::readFromFile - Non-regular File", "[file]") {
    const std::string dirName = "testdir";

    fs::create_directory(dirName);

    SECTION("Read from a directory") {
        REQUIRE_THROWS_MATCHES(vnd::readFromFile(dirName), std::runtime_error, MSG_FORMAT("Path is not a regular file: {}", dirName));
    }

    [[maybe_unused]] auto unsed = fs::remove(dirName);
}

TEST_CASE("vnd::readFromFile - Empty File", "[file]") {
    const std::string emtfilename = "emptyfile.txt";

    createFile(emtfilename, "");

    SECTION("Read from an empty file") {
        const auto result = vnd::readFromFile(emtfilename);
        REQUIRE(result.empty());  // Ensure the result is empty
    }

    [[maybe_unused]] auto unsed = fs::remove(emtfilename);
}

TEST_CASE("vnd::readFromFile - Large File", "[file]") {
    const std::string lrgfilename = "largefile.txt";
    const std::string largeContent(C_ST(1024 * 1024) * 10, 'a');  // 10 MB of 'a'

    createFile(lrgfilename, largeContent);

    SECTION("Read from a large file") {
        auto result = vnd::readFromFile(lrgfilename);
        REQUIRE(result == largeContent);  // Ensure content matches
    }

    [[maybe_unused]] auto unsed = fs::remove(lrgfilename);
}

/*TEST_CASE("FolderCreationResult Serialization") {
    vnd::FolderCreationResult result(true, fs::path("/test/path"));
    nlohmann::json jsn = result;
    REQUIRE(jsn["success"] == true);
    REQUIRE(jsn["path"] == "/test/path");

    vnd::FolderCreationResult deserializedResult;
    jsn.get_to(deserializedResult);
    REQUIRE(deserializedResult.success() == true);
    REQUIRE(deserializedResult.path() == fs::path("/test/path"));
}*/
TEST_CASE("default constructed token set propriety format", "[token]") {
    using enum vnd::TokenType;
    vnd::Token token{};
    REQUIRE(token.getType() == UNKNOWN);
    REQUIRE(token.getValue().empty() == true);
    REQUIRE(token.getLine() == 0);
    REQUIRE(token.getColumn() == 0);
    REQUIRE(FORMAT("{}", token) == "(type: UNKNOWN, value: '', source location:(file:unknown, line:0, column:0))");
    modifyToken(token);
    REQUIRE(token.getType() == INTEGER);
    REQUIRE(token.getValue().empty() == false);
    REQUIRE(token.getFileName() == filename);
    REQUIRE(token.getLine() == 1);
    REQUIRE(token.getColumn() == 1);
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    REQ_FORMAT(token, R"((type: INTEGER, value: 'assss', source location:(file:.\unknown.vn, line:1, column:1)))");
#else
    REQ_FORMAT(token, R"((type: INTEGER, value: 'assss', source location:(file:./unknown.vn, line:1, column:1)))");
#endif
}

TEST_CASE("Token Comparison Equality", "[Token]") {
    vnd::Token token1(vnd::TokenType::PLUS, "+", vnd::CodeSourceLocation(filename, t_line, t_colum));
    vnd::Token token2(vnd::TokenType::PLUS, "+", vnd::CodeSourceLocation(filename, t_line, t_colum));
    REQUIRE(token1 == token2);
}

TEST_CASE("Token Comparison Inequality", "[Token]") {
    vnd::Token token1(identf, "variable", vnd::CodeSourceLocation(filename, t_line, t_colum));
    vnd::Token token2(identf, "variable2", vnd::CodeSourceLocation(filename, t_line, t_colum));
    REQUIRE(token1 != token2);
}

TEST_CASE("TokenizerUtility CommaOrColonType", "[tokenizer]") {
    using enum vnd::TokenType;
    REQUIRE(vnd::TokenizerUtility::CommaOrColonType('a') == UNKNOWN);
    REQUIRE(vnd::TokenizerUtility::CommaOrColonType(',') == COMMA);
    REQUIRE(vnd::TokenizerUtility::CommaOrColonType(':') == COLON);
}

TEST_CASE("TokenizerUtility CommaOrColonValue", "[tokenizer]") {
    REQUIRE(vnd::TokenizerUtility::CommaOrColonValue('a') == "unknown");
    REQUIRE(vnd::TokenizerUtility::CommaOrColonValue(',') == ",");
    REQUIRE(vnd::TokenizerUtility::CommaOrColonValue(':') == ":");
}

TEST_CASE("tokenizer emit identifier token", "[tokenizer]") {
    vnd::Tokenizer tokenizer{"a a_ a0 a000_ _a", filename};
    std::vector<vnd::TokenVec> result = tokenizer.tokenize();
    REQUIRE(result.size() == 1);
    vnd::TokenVec tokens = result.front();
    REQUIRE(tokens.size() == 6);
    REQUIRE(tokens[0] == vnd::Token(identf, "a", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(identf, "a_", vnd::CodeSourceLocation(filename, 1, 3)));
    REQUIRE(tokens[2] == vnd::Token(identf, "a0", vnd::CodeSourceLocation(filename, 1, 6)));
    REQUIRE(tokens[3] == vnd::Token(identf, "a000_", vnd::CodeSourceLocation(filename, 1, t_colum5)));
    REQUIRE(tokens[4] == vnd::Token(identf, "_a", vnd::CodeSourceLocation(filename, 1, t_colum10)));
}

TEST_CASE("tokenizer emit identifier token new line", "[tokenizer]") {
    vnd::Tokenizer tokenizer{"a a_\na0 a000_ _a", filename};
    std::vector<vnd::TokenVec> result = tokenizer.tokenize();
    REQUIRE(result.size() == 2);
    vnd::TokenVec tokens = result.at(0);
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0] == vnd::Token(identf, "a", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(identf, "a_", vnd::CodeSourceLocation(filename, 1, 3)));
    tokens = result.at(1);
    REQUIRE(tokens.size() == 4);
    REQUIRE(tokens[0] == vnd::Token(identf, "a0", vnd::CodeSourceLocation(filename, 2, 1)));
    REQUIRE(tokens[1] == vnd::Token(identf, "a000_", vnd::CodeSourceLocation(filename, 2, 4)));
    REQUIRE(tokens[2] == vnd::Token(identf, "_a", vnd::CodeSourceLocation(filename, 2, 10)));
    REQUIRE(tokens[3].getType() == vnd::eofTokenType);
}

TEST_CASE("tokenizer emit integer token for hexadecimals numbers", "[tokenizer]") {
    // hexadecimals 0xhexnum a-f A-F 0-9
    vnd::Tokenizer tokenizer{"#0 #23 #24 #ff #7f", filename};
    std::vector<vnd::TokenVec> result = tokenizer.tokenize();
    REQUIRE(result.size() == 1);
    vnd::TokenVec tokens = result.front();
    REQUIRE(tokens.size() == 6);
    REQUIRE(tokens[0] == vnd::Token(inte, "#0", vnd::CodeSourceLocation(filename, 1, 0)));
    REQUIRE(tokens[1] == vnd::Token(inte, "#23", vnd::CodeSourceLocation(filename, 1, 3)));
    REQUIRE(tokens[2] == vnd::Token(inte, "#24", vnd::CodeSourceLocation(filename, 1, t_colum3)));
    REQUIRE(tokens[3] == vnd::Token(inte, "#ff", vnd::CodeSourceLocation(filename, 1, t_colum8)));
    REQUIRE(tokens[4] == vnd::Token(inte, "#7f", vnd::CodeSourceLocation(filename, 1, t_colum10)));
}

TEST_CASE("tokenizer emit exception on  malformed exadecimal number or octal number", "[tokenizer]") {
    vnd::Tokenizer tokenizer{"#", filename};
    REQUIRE_THROWS_MATCHES(tokenizer.tokenize(), std::runtime_error,
                           MessageMatches(ContainsSubstring("malformed exadecimal number or octal number '#' (line 1, column 2):")));
}

TEST_CASE("tokenizer emit integer token for octal numbers", "[tokenizer]") {
    // octal 0oOctnum 0-7
    vnd::Tokenizer tokenizer{"#o0 #o23 #o24", filename};
    std::vector<vnd::TokenVec> result = tokenizer.tokenize();
    REQUIRE(result.size() == 1);
    vnd::TokenVec tokens = result.front();
    REQUIRE(tokens.size() == 4);
    REQUIRE(tokens[0] == vnd::Token(inte, "#o0", vnd::CodeSourceLocation(filename, 1, 0)));
    REQUIRE(tokens[1] == vnd::Token(inte, "#o23", vnd::CodeSourceLocation(filename, 1, 4)));
    REQUIRE(tokens[2] == vnd::Token(inte, "#o24", vnd::CodeSourceLocation(filename, 1, t_colum5)));
}

TEST_CASE("tokenizer emit integer token", "[tokenizer]") {
    vnd::Tokenizer tokenizer{"42 333 550 34000000", filename};
    std::vector<vnd::TokenVec> result = tokenizer.tokenize();
    REQUIRE(result.size() == 1);
    vnd::TokenVec tokens = result.front();
    REQUIRE(tokens.size() == 5);
    REQUIRE(tokens[0] == vnd::Token(inte, "42", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(inte, "333", vnd::CodeSourceLocation(filename, 1, 4)));
    REQUIRE(tokens[2] == vnd::Token(inte, "550", vnd::CodeSourceLocation(filename, 1, t_colum4)));
    REQUIRE(tokens[3] == vnd::Token(inte, "34000000", vnd::CodeSourceLocation(filename, 1, t_colum14)));
}

TEST_CASE("tokenizer emit integer token new line", "[tokenizer]") {
    vnd::Tokenizer tokenizer{"42 333\n550 34000000", filename};
    std::vector<vnd::TokenVec> result = tokenizer.tokenize();
    REQUIRE(result.size() == 2);
    vnd::TokenVec tokens = result.at(0);
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0] == vnd::Token(inte, "42", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(inte, "333", vnd::CodeSourceLocation(filename, 1, 4)));
    tokens = result.at(1);
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == vnd::Token(inte, "550", vnd::CodeSourceLocation(filename, 2, 1)));
    REQUIRE(tokens[1] == vnd::Token(inte, "34000000", vnd::CodeSourceLocation(filename, 2, 5)));
    REQUIRE(tokens[2].getType() == vnd::eofTokenType);
}

TEST_CASE("tokenizer emit double token", "[tokenizer]") {
    vnd::Tokenizer tokenizer{"1. 1.0 1e+1 1E+1 1.1e+1 1.1E+1 1e-1 1E-1 1.1e-1 1.1E-1 .4e12 4i 5.4if .7f", filename};
    std::vector<vnd::TokenVec> result = tokenizer.tokenize();
    REQUIRE(result.size() == 1);
    vnd::TokenVec tokens = result.front();
    REQUIRE(tokens.size() == 15);
    REQUIRE(tokens[0] == vnd::Token(doub, "1.", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(doub, "1.0", vnd::CodeSourceLocation(filename, 1, 4)));
    REQUIRE(tokens[2] == vnd::Token(doub, "1e+1", vnd::CodeSourceLocation(filename, 1, t_colum4)));
    REQUIRE(tokens[3] == vnd::Token(doub, "1E+1", vnd::CodeSourceLocation(filename, 1, t_colum11)));
    REQUIRE(tokens[4] == vnd::Token(doub, "1.1e+1", vnd::CodeSourceLocation(filename, 1, 18)));
    REQUIRE(tokens[5] == vnd::Token(doub, "1.1E+1", vnd::CodeSourceLocation(filename, 1, 25)));
    REQUIRE(tokens[6] == vnd::Token(doub, "1e-1", vnd::CodeSourceLocation(filename, 1, 32)));
    REQUIRE(tokens[7] == vnd::Token(doub, "1E-1", vnd::CodeSourceLocation(filename, 1, 37)));
    REQUIRE(tokens[8] == vnd::Token(doub, "1.1e-1", vnd::CodeSourceLocation(filename, 1, 42)));
    REQUIRE(tokens[9] == vnd::Token(doub, "1.1E-1", vnd::CodeSourceLocation(filename, 1, 49)));
    REQUIRE(tokens[10] == vnd::Token(doub, ".4e12", vnd::CodeSourceLocation(filename, 1, 56)));
    REQUIRE(tokens[11] == vnd::Token(doub, "4i", vnd::CodeSourceLocation(filename, 1, 62)));
    REQUIRE(tokens[12] == vnd::Token(doub, "5.4if", vnd::CodeSourceLocation(filename, 1, 65)));
    REQUIRE(tokens[13] == vnd::Token(doub, ".7f", vnd::CodeSourceLocation(filename, 1, 71)));
}

TEST_CASE("tokenizer emit operator token", "[tokenizer]") {
    using enum vnd::TokenType;
    vnd::Tokenizer tokenizer{"* / = , : < > ! | & + - ^ . %", filename};
    std::vector<vnd::TokenVec> result = tokenizer.tokenize();
    REQUIRE(result.size() == 1);
    vnd::TokenVec tokens = result.front();
    REQUIRE(tokens.size() == 16);
    REQUIRE(tokens[0] == vnd::Token(STAR, "*", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(DIVIDE, "/", vnd::CodeSourceLocation(filename, 1, 3)));
    REQUIRE(tokens[2] == vnd::Token(EQUAL, "=", vnd::CodeSourceLocation(filename, 1, 5)));
    REQUIRE(tokens[3] == vnd::Token(COMMA, ",", vnd::CodeSourceLocation(filename, 1, t_colum + 1)));
    REQUIRE(tokens[4] == vnd::Token(COLON, ":", vnd::CodeSourceLocation(filename, 1, t_colum4 + 1)));
    // REQUIRE(tokens[5].getColumn() == t_colum8);
    REQUIRE(tokens[5] == vnd::Token(LESS, "<", vnd::CodeSourceLocation(filename, 1, t_colum8)));
    REQUIRE(tokens[6] == vnd::Token(GREATER, ">", vnd::CodeSourceLocation(filename, 1, t_colum11)));
    REQUIRE(tokens[7] == vnd::Token(NOT, "!", vnd::CodeSourceLocation(filename, 1, t_colum10)));
    REQUIRE(tokens[8] == vnd::Token(OR, "|", vnd::CodeSourceLocation(filename, 1, t_colum13)));
    REQUIRE(tokens[9] == vnd::Token(AND, "&", vnd::CodeSourceLocation(filename, 1, 19)));
    REQUIRE(tokens[10] == vnd::Token(PLUS, "+", vnd::CodeSourceLocation(filename, 1, 21)));
    REQUIRE(tokens[11] == vnd::Token(MINUS, "-", vnd::CodeSourceLocation(filename, 1, 23)));
    REQUIRE(tokens[12] == vnd::Token(XOR, "^", vnd::CodeSourceLocation(filename, 1, 25)));
    REQUIRE(tokens[13] == vnd::Token(DOT, ".", vnd::CodeSourceLocation(filename, 1, 27)));
    REQUIRE(tokens[14] == vnd::Token(PERCENT, "%", vnd::CodeSourceLocation(filename, 1, 29)));
}

TEST_CASE("tokenizer emit operationEqual token", "[tokenizer]") {
    using enum vnd::TokenType;
    vnd::Tokenizer tokenizer{"+= -= *= /= %=", filename};
    std::vector<vnd::TokenVec> result = tokenizer.tokenize();
    REQUIRE(result.size() == 1);
    vnd::TokenVec tokens = result.front();
    REQUIRE(tokens.size() == 6);
    REQUIRE(tokens[0] == vnd::Token(PLUSEQUAL, "+=", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(MINUSEQUAL, "-=", vnd::CodeSourceLocation(filename, 1, 4)));
    REQUIRE(tokens[2] == vnd::Token(STAREQUAL, "*=", vnd::CodeSourceLocation(filename, 1, t_colum3)));
    REQUIRE(tokens[3] == vnd::Token(DIVIDEEQUAL, "/=", vnd::CodeSourceLocation(filename, 1, 10)));
    REQUIRE(tokens[4] == vnd::Token(PERCENTEQUAL, "%=", vnd::CodeSourceLocation(filename, 1, 13)));
}

TEST_CASE("tokenizer emit boolean operator token", "[tokenizer]") {
    using enum vnd::TokenType;
    vnd::Tokenizer tokenizer{"== >= <= !=", filename};
    std::vector<vnd::TokenVec> result = tokenizer.tokenize();
    REQUIRE(result.size() == 1);
    vnd::TokenVec tokens = result.front();
    REQUIRE(tokens.size() == 5);
    REQUIRE(tokens[0] == vnd::Token(EQUALEQUAL, "==", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(GREATEREQUAL, ">=", vnd::CodeSourceLocation(filename, 1, 4)));
    REQUIRE(tokens[2] == vnd::Token(LESSEQUAL, "<=", vnd::CodeSourceLocation(filename, 1, t_colum3)));
    REQUIRE(tokens[3] == vnd::Token(NOTEQUAL, "!=", vnd::CodeSourceLocation(filename, 1, 10)));
}

TEST_CASE("tokenizer emit logical operator token", "[tokenizer]") {
    vnd::Tokenizer tokenizer{"&& ||", filename};
    std::vector<vnd::TokenVec> result = tokenizer.tokenize();
    REQUIRE(result.size() == 1);
    vnd::TokenVec tokens = result.front();
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::ANDAND, "&&", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(vnd::TokenType::OROR, "||", vnd::CodeSourceLocation(filename, 1, 4)));
}

TEST_CASE("tokenizer emit unary operator token", "[tokenizer]") {
    vnd::Tokenizer tokenizer{"++ --", filename};
    std::vector<vnd::TokenVec> result = tokenizer.tokenize();
    REQUIRE(result.size() == 1);
    vnd::TokenVec tokens = result.front();
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::PLUSPLUS, "++", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(vnd::TokenType::MINUSMINUS, "--", vnd::CodeSourceLocation(filename, 1, 4)));
}

TEST_CASE("tokenizer emit parenthesis token", "[tokenizer]") {
    vnd::Tokenizer tokenizer{"( )", filename};
    std::vector<vnd::TokenVec> result = tokenizer.tokenize();
    REQUIRE(result.size() == 1);
    vnd::TokenVec tokens = result.front();
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::OPEN_PARENTESIS, "(", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(vnd::TokenType::CLOSE_PARENTESIS, ")", vnd::CodeSourceLocation(filename, 1, 3)));
}

TEST_CASE("tokenizer emit square parenthesis token", "[tokenizer]") {
    vnd::Tokenizer tokenizer{"[ ]", filename};
    std::vector<vnd::TokenVec> result = tokenizer.tokenize();
    REQUIRE(result.size() == 1);
    vnd::TokenVec tokens = result.front();
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::OPEN_SQ_PARENTESIS, "[", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(vnd::TokenType::CLOSE_SQ_PARENTESIS, "]", vnd::CodeSourceLocation(filename, 1, 3)));
}

TEST_CASE("Tokenizer emit square curly token", "[Tokenizer]") {
    vnd::Tokenizer tokenizer{"{ }", filename};
    std::vector<vnd::TokenVec> result = tokenizer.tokenize();
    REQUIRE(result.size() == 1);
    vnd::TokenVec tokens = result.front();
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::OPEN_CUR_PARENTESIS, "{", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(vnd::TokenType::CLOSE_CUR_PARENTESIS, "}", vnd::CodeSourceLocation(filename, 1, 3)));
}

TEST_CASE("Tokenizer emit char token", "[Tokenizer]") {
    using enum vnd::TokenType;
    constexpr std::string_view code2 = R"('a' '\\' '')";
    vnd::Tokenizer tokenizer{code2, filename};
    std::vector<vnd::TokenVec> result = tokenizer.tokenize();
    REQUIRE(result.size() == 1);
    vnd::TokenVec tokens = result.front();
    REQUIRE(tokens.size() == 4);
    REQUIRE(tokens[0] == vnd::Token(CHAR, "a", vnd::CodeSourceLocation(filename, 1, 2)));
    REQUIRE(tokens[1] == vnd::Token(CHAR, R"(\\)", vnd::CodeSourceLocation(filename, 1, t_colum)));
    REQUIRE(tokens[2] == vnd::Token(CHAR, "", vnd::CodeSourceLocation(filename, 1, t_colum8)));
}

TEST_CASE("Tokenizer emit exception for unknown char", "[Tokenizer]") {
    vnd::Tokenizer tokenizer{";", filename};
    REQUIRE_THROWS_MATCHES(tokenizer.tokenize(), std::runtime_error,
                           MessageMatches(ContainsSubstring("Unknown Character ';' (line 1, column 1):")));
}

TEST_CASE("Tokenizer emit string token", "[Tokenizer]") {
    using enum vnd::TokenType;
    constexpr std::string_view code2 = R"("a" "\\" "")";
    vnd::Tokenizer tokenizer{code2, filename};
    std::vector<vnd::TokenVec> result = tokenizer.tokenize();
    REQUIRE(result.size() == 1);
    vnd::TokenVec tokens = result.front();
    REQUIRE(tokens.size() == 4);
    REQUIRE(tokens[0] == vnd::Token(STRING, "a", vnd::CodeSourceLocation(filename, 1, 1)));
    REQUIRE(tokens[1] == vnd::Token(STRING, R"(\\)", vnd::CodeSourceLocation(filename, 1, 5)));
    REQUIRE(tokens[2] == vnd::Token(STRING, "", vnd::CodeSourceLocation(filename, 1, 10)));
}

TEST_CASE("tokenizer emit unknown token on non closed char token", "[tokenizer]") {
    constexpr std::string_view code2 = R"('a")";
    vnd::Tokenizer tokenizer{code2, filename};
    std::vector<vnd::TokenVec> result = tokenizer.tokenize();
    REQUIRE(result.size() == 1);
    vnd::TokenVec tokens = result.front();
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::UNKNOWN, R"(a")", vnd::CodeSourceLocation(filename, 1, 2)));
}

TEST_CASE("tokenizer emit comment token", "[tokenizer]") {
    SECTION("Basic single-line comment") {
        constexpr std::string_view code2 = R"(// line comment)";
        vnd::Tokenizer tokenizer{code2, filename};
        std::vector<vnd::TokenVec> result = tokenizer.tokenize();
        REQUIRE(result.size() == 1);
        vnd::TokenVec tokens = result.front();
        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::COMMENT, "// line comment", vnd::CodeSourceLocation(filename, 1, 1)));
    }

    SECTION("Single-line comment with symbols") {
        const std::string input = "// Comment with symbols !@#$%^&*()123456\n";
        vnd::Tokenizer tokenizer{input, filename};

        std::vector<vnd::TokenVec> result = tokenizer.tokenize();
        REQUIRE(result.size() == 2);
        vnd::TokenVec tokens = result.front();
        REQUIRE(tokens.size() == 1);
        REQUIRE(tokens[0].getType() == vnd::TokenType::COMMENT);
        REQUIRE(tokens[0].getValue() == "// Comment with symbols !@#$%^&*()123456");
        REQUIRE(result.at(1).front().getType() == vnd::eofTokenType);
    }

    SECTION("Single-line comment with no text") {
        const std::string input = "//\n";
        vnd::Tokenizer tokenizer{input, filename};

        std::vector<vnd::TokenVec> result = tokenizer.tokenize();
        REQUIRE(result.size() == 2);
        vnd::TokenVec tokens = result.front();
        REQUIRE(tokens.size() == 1);
        REQUIRE(tokens[0].getType() == vnd::TokenType::COMMENT);
        REQUIRE(tokens[0].getValue() == "//");
        REQUIRE(result.at(1).front().getType() == vnd::eofTokenType);
    }
}

TEST_CASE("tokenizer emit multiline comment token", "[tokenizer]") {
    SECTION("Basic multi-line comment") {
        constexpr std::string_view code2 = R"(/*multi\nline\ncomment*/)";
        vnd::Tokenizer tokenizer{code2, filename};
        std::vector<vnd::TokenVec> result = tokenizer.tokenize();
        REQUIRE(result.size() == 1);
        vnd::TokenVec tokens = result.front();
        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0] == vnd::Token(vnd::TokenType::COMMENT, R"(/*multi\nline\ncomment*/)", vnd::CodeSourceLocation(filename, 1, 1)));
    }

    SECTION("Multi-line comment with asterisks inside") {
        const std::string input = "/* This * is * a * multi-line comment */";
        vnd::Tokenizer tokenizer{input, filename};
        std::vector<vnd::TokenVec> result = tokenizer.tokenize();
        REQUIRE(result.size() == 1);
        vnd::TokenVec tokens = result.front();
        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0].getType() == vnd::TokenType::COMMENT);
        REQUIRE(tokens[0].getValue() == "/* This * is * a * multi-line comment */");
    }
}

TEST_CASE("tokenizer emit mixed Comments", "[tokenizer]") {
    SECTION("Single and multi-line comments") {
        const std::string input = "// Single line\n/* Multi-line */";
        vnd::Tokenizer tokenizer(input, "testFile");

        std::vector<vnd::TokenVec> result = tokenizer.tokenize();
        REQUIRE(result.size() == 2);
        vnd::TokenVec tokens = result.at(0);
        REQUIRE(tokens[0].getType() == vnd::TokenType::COMMENT);
        REQUIRE(tokens[0].getValue() == "// Single line");
        tokens = result.at(1);
        REQUIRE(tokens[0].getType() == vnd::TokenType::COMMENT);
        REQUIRE(tokens[0].getValue() == "/* Multi-line */");
    }

    SECTION("Multi-line comment followed by single-line comment") {
        const std::string input = "/* Multi-line */\n// Single line";
        vnd::Tokenizer tokenizer(input, "testFile");

        std::vector<vnd::TokenVec> result = tokenizer.tokenize();
        REQUIRE(result.size() == 2);
        vnd::TokenVec tokens = result.at(0);
        REQUIRE(tokens[0].getType() == vnd::TokenType::COMMENT);
        REQUIRE(tokens[0].getValue() == "/* Multi-line */");
        tokens = result.at(1);
        REQUIRE(tokens[0].getType() == vnd::TokenType::COMMENT);
        REQUIRE(tokens[0].getValue() == "// Single line");
    }
}

TEST_CASE("tokenizer edge cases for comments", "[tokenizer]") {
    const std::string input = "/* Comment with ** inside */";
    vnd::Tokenizer tokenizer(input, "testFile");

    auto token = tokenizer.tokenize().front().front();
    REQUIRE(token.getType() == vnd::TokenType::COMMENT);
    REQUIRE(token.getValue() == "/* Comment with ** inside */");
}

TEST_CASE("ASTNode type conversion using as<T>()", "[ast]") {
    const vnd::Token token{vnd::TokenType::IDENTIFIER, "id", vnd::CodeSourceLocation{filename, t_line, t_colum}};
    vnd::VariableNode dummyNode("id", token);

    vnd::ASTNode *basePtr = &dummyNode;
    auto *dummyPtr = basePtr->as<vnd::VariableNode>();
    REQUIRE(dummyPtr != nullptr);

    auto *anotherPtr = basePtr->as<vnd::Token>();
    REQUIRE(anotherPtr == nullptr);

    const vnd::ASTNode *constBasePtr = &dummyNode;
    const auto *constDummyPtr = constBasePtr->as<vnd::VariableNode>();
    REQUIRE(constDummyPtr != nullptr);

    const auto *constAnotherPtr = constBasePtr->as<vnd::Token>();
    REQUIRE(constAnotherPtr == nullptr);
}

TEST_CASE("ASTNode type conversion using safe_as<T>()", "[ast]") {
    const vnd::Token token{vnd::TokenType::IDENTIFIER, "id", vnd::CodeSourceLocation{filename, t_line, t_colum}};
    vnd::VariableNode dummyNode("id", token);

    vnd::ASTNode *basePtr = &dummyNode;
    auto *dummyPtr = basePtr->as<vnd::VariableNode>();
    REQUIRE(dummyPtr != nullptr);

    auto *anotherPtr = basePtr->as<vnd::Token>();
    REQUIRE(anotherPtr == nullptr);

    const vnd::ASTNode *constBasePtr = &dummyNode;
    const auto *constDummyPtr = constBasePtr->safe_as<vnd::VariableNode>();
    REQUIRE(constDummyPtr != nullptr);

    const auto *constAnotherPtr = constBasePtr->safe_as<vnd::Token>();
    REQUIRE(constAnotherPtr == nullptr);
}

TEST_CASE("ASTNode type conversion using safe_as<T>() non  const", "[ast]") {
    const vnd::Token token{vnd::TokenType::IDENTIFIER, "id", vnd::CodeSourceLocation{filename, t_line, t_colum}};
    vnd::VariableNode dummyNode("id", token);

    vnd::ASTNode *basePtr = &dummyNode;
    auto *dummyPtr = basePtr->as<vnd::VariableNode>();
    REQUIRE(dummyPtr != nullptr);

    auto *anotherPtr = basePtr->as<vnd::Token>();
    REQUIRE(anotherPtr == nullptr);

    auto *constBasePtr = &dummyNode;
    auto *constDummyPtr = constBasePtr->safe_as<vnd::VariableNode>();
    REQUIRE(constDummyPtr != nullptr);

    auto *constAnotherPtr = constBasePtr->safe_as<vnd::Token>();
    REQUIRE(constAnotherPtr == nullptr);
}

TEST_CASE("ASTNode get token", "[ast]") {
    vnd::Token token{vnd::TokenType::IDENTIFIER, "id", vnd::CodeSourceLocation{filename, t_line, t_colum}};
    const vnd::VariableNode dummyNode("id", token);
    REQUIRE(dummyNode.get_token() == token);
}
TEST_CASE("Parser emit boolean literal node", "[parser]") {
    vnd::Parser parser("true", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Boolean);
    const auto *number = ast->as<vnd::LiteralNode<bool>>();
    REQUIRE(number != nullptr);
    REQUIRE(number->print() == "BOOLEAN_LIT(true)");
    REQUIRE(number->comp_print() == "BOOLEAN(true)");
    REQUIRE(number->get_value() == true);
}

TEST_CASE("boolean node swap", "[parser]") {
    auto token1 = vnd::Token{vnd::TokenType::BOOLEAN, "true", vnd::CodeSourceLocation{filename, t_line, t_colum4}};
    auto token2 = vnd::Token{vnd::TokenType::BOOLEAN, "false", vnd::CodeSourceLocation{filename, t_line, t_colum2}};
    vnd::LiteralNode<bool> boola{true, token1, NodeType::Boolean};
    vnd::LiteralNode<bool> boolb{false, token2, NodeType::Boolean};
    REQUIRE(boola.get_value() == true);
    REQUIRE(boolb.get_value() == false);
    REQUIRE(boola.get_token() == token1);
    REQUIRE(boolb.get_token() == token2);
    std::swap(boola, boolb);
    REQUIRE(boola.get_value() == false);
    REQUIRE(boolb.get_value() == true);
    REQUIRE(boola.get_token() == token2);
    REQUIRE(boolb.get_token() == token1);
}

TEST_CASE("Parser emit string literal node", "[parser]") {
    vnd::Parser parser("\"tr\"", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::String);
    const auto *number = ast->as<vnd::LiteralNode<std::string_view>>();
    REQUIRE(number != nullptr);
    REQUIRE(number->print() == "STRING_LIT(tr)");
    REQUIRE(number->comp_print() == "STRING(tr)");
    REQUIRE(number->get_value() == "tr");
}

TEST_CASE("string node swap", "[parser]") {
    auto token1 = vnd::Token{vnd::TokenType::STRING, "tr", vnd::CodeSourceLocation{filename, t_line, t_colum4}};
    auto token2 = vnd::Token{vnd::TokenType::STRING, "sr", vnd::CodeSourceLocation{filename, t_line, t_colum2}};
    vnd::LiteralNode<std::string_view> boola{"tr", token1, NodeType::String};
    vnd::LiteralNode<std::string_view> boolb{"sr", token2, NodeType::String};
    REQUIRE(boola.get_value() == "tr");
    REQUIRE(boolb.get_value() == "sr");
    REQUIRE(boola.get_token() == token1);
    REQUIRE(boolb.get_token() == token2);
    std::swap(boola, boolb);
    REQUIRE(boola.get_value() == "sr");
    REQUIRE(boolb.get_value() == "tr");
    REQUIRE(boola.get_token() == token2);
    REQUIRE(boolb.get_token() == token1);
}

TEST_CASE("Parser emit char literal node", "[parser]") {
    vnd::Parser parser("'c'", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Char);
    const auto *number = ast->as<vnd::LiteralNode<char>>();
    REQUIRE(number != nullptr);
    REQUIRE(number->print() == "CHAR_LIT(c)");
    REQUIRE(number->comp_print() == "CHAR(c)");
    REQUIRE(number->get_value() == 'c');
}

TEST_CASE("char node swap", "[parser]") {
    auto token1 = vnd::Token{vnd::TokenType::CHAR, "c", vnd::CodeSourceLocation{filename, t_line, t_colum4}};
    auto token2 = vnd::Token{vnd::TokenType::CHAR, "d", vnd::CodeSourceLocation{filename, t_line, t_colum2}};
    vnd::LiteralNode<char> boola{'c', token1, NodeType::Char};
    vnd::LiteralNode<char> boolb{'d', token2, NodeType::Char};
    REQUIRE(boola.get_value() == 'c');
    REQUIRE(boolb.get_value() == 'd');
    REQUIRE(boola.get_token() == token1);
    REQUIRE(boolb.get_token() == token2);
    std::swap(boola, boolb);
    REQUIRE(boola.get_value() == 'd');
    REQUIRE(boolb.get_value() == 'c');
    REQUIRE(boola.get_token() == token2);
    REQUIRE(boolb.get_token() == token1);
}

TEST_CASE("Parser emit integer number node", "[parser]") {
    vnd::Parser parser("1", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<VND_NUM_INT>();
    REQUIRE(number != nullptr);
    REQUIRE(number->get_value() == 1);
}

TEST_CASE("Parser emit complex number node", "[parser]") {
    vnd::Parser parser("1i", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<VND_NUM_CDOUBLE>();
    REQUIRE(number != nullptr);
    REQUIRE(number->print() == "NUMBER_IMAGINARY((0, 1))");
    REQUIRE(number->comp_print() == "NUM_IMG((0, 1))");
    REQUIRE(number->get_value() == std::complex<double>(0, 1));
}

TEST_CASE("Parser emit complex float number node", "[parser]") {
    vnd::Parser parser("1if", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<VND_NUM_CFLOAT>();
    REQUIRE(number != nullptr);
    REQUIRE(number->print() == "NUMBER_IMAGINARY_F((0, 1))");
    REQUIRE(number->comp_print() == "NUM_IMF((0, 1))");
    REQUIRE(number->get_value() == std::complex<float>(0, 1));
}

TEST_CASE("integer number node swap", "[parser]") {
    auto token1 = vnd::Token{vnd::TokenType::INTEGER, "2", vnd::CodeSourceLocation{filename, t_line, t_colum}};
    auto token2 = vnd::Token{vnd::TokenType::INTEGER, "3", vnd::CodeSourceLocation{filename, t_line, t_colum2}};
    vnd::NumberNode<int> inta{2, token1, NumberNodeType::Integer};
    vnd::NumberNode<int> intb{3, token2, NumberNodeType::Integer};
    REQUIRE(inta.get_value() == 2);
    REQUIRE(intb.get_value() == 3);
    REQUIRE(inta.get_token() == token1);
    REQUIRE(intb.get_token() == token2);
    std::swap(inta, intb);
    REQUIRE(inta.get_value() == 3);
    REQUIRE(intb.get_value() == 2);
    REQUIRE(inta.get_token() == token2);
    REQUIRE(intb.get_token() == token1);
}

TEST_CASE("Imaginary number node swap", "[parser]") {
    auto token1 = vnd::Token{vnd::TokenType::DOUBLE, "2if", vnd::CodeSourceLocation{filename, t_line, t_colum}};
    auto token2 = vnd::Token{vnd::TokenType::DOUBLE, "3if", vnd::CodeSourceLocation{filename, t_line, t_colum3}};
    vnd::NumberNode<std::complex<float>> cmpa{std::complex<float>(0, 2), token1, NumberNodeType::ImaginaryFloat};
    vnd::NumberNode<std::complex<float>> cmpb{std::complex<float>(0, 3), token2, NumberNodeType::ImaginaryFloat};
    REQUIRE(cmpa.get_value() == std::complex<float>(0, 2));
    REQUIRE(cmpb.get_value() == std::complex<float>(0, 3));
    REQUIRE(cmpa.get_token() == token1);
    REQUIRE(cmpb.get_token() == token2);
    std::swap(cmpa, cmpb);
    REQUIRE(cmpa.get_value() == std::complex<float>(0, 3));
    REQUIRE(cmpb.get_value() == std::complex<float>(0, 2));
    REQUIRE(cmpa.get_token() == token2);
    REQUIRE(cmpb.get_token() == token1);
}

TEST_CASE("double number node swap", "[parser]") {
    vnd::NumberNode<double> douba{2.3, vnd::Token{vnd::TokenType::DOUBLE, "2.3", vnd::CodeSourceLocation{filename, t_line, t_colum}},
                                  NumberNodeType::Double};
    vnd::NumberNode<double> doubb{3.3, vnd::Token{vnd::TokenType::DOUBLE, "3.3", vnd::CodeSourceLocation{filename, t_line, t_colum2}},
                                  NumberNodeType::Double};
    REQUIRE(douba.get_value() == 2.3);
    REQUIRE(doubb.get_value() == 3.3);
    std::swap(douba, doubb);
    REQUIRE(douba.get_value() == 3.3);
    REQUIRE(doubb.get_value() == 2.3);
}

TEST_CASE("unary node swap", "[parser]") {
    using enum vnd::TokenType;
    auto token1 = vnd::Token{MINUS, "-", vnd::CodeSourceLocation{filename, t_line, t_colum}};
    auto token2 = vnd::Token{IDENTIFIER, "d", vnd::CodeSourceLocation{filename, t_line, t_colum6}};
    auto token3 = vnd::Token{STAR, "*", vnd::CodeSourceLocation{filename, t_line4, t_colum}};
    auto token4 = vnd::Token{IDENTIFIER, "s", vnd::CodeSourceLocation{filename, t_line4, t_colum4}};
    vnd::UnaryExpressionNode unara{"-", token1, MAKE_UNIQUE(vnd::VariableNode, "d", token2)};
    vnd::UnaryExpressionNode unarb{"*", token3, MAKE_UNIQUE(vnd::VariableNode, "s", token4)};
    REQUIRE(unara.getOp() == "-");
    REQUIRE(unara.getOperand()->as<vnd::VariableNode>()->getName() == "d");
    REQUIRE(unara.get_token() == token1);
    REQUIRE(unara.getOperand()->as<vnd::VariableNode>()->get_token() == token2);
    REQUIRE(unarb.getOp() == "*");
    REQUIRE(unarb.getOperand()->as<vnd::VariableNode>()->getName() == "s");
    REQUIRE(unarb.get_token() == token3);
    REQUIRE(unarb.getOperand()->as<vnd::VariableNode>()->get_token() == token4);
    std::swap(unara, unarb);
    REQUIRE(unara.getOp() == "*");
    REQUIRE(unara.getOperand()->as<vnd::VariableNode>()->getName() == "s");
    REQUIRE(unara.get_token() == token3);
    REQUIRE(unara.getOperand()->as<vnd::VariableNode>()->get_token() == token4);
    REQUIRE(unarb.getOp() == "-");
    REQUIRE(unarb.getOperand()->as<vnd::VariableNode>()->getName() == "d");
    REQUIRE(unarb.get_token() == token1);
    REQUIRE(unarb.getOperand()->as<vnd::VariableNode>()->get_token() == token2);
}

TEST_CASE("binary node swap", "[parser]") {
    using enum vnd::TokenType;
    auto token1 = vnd::Token{MINUS, "-", vnd::CodeSourceLocation{filename, t_line, t_colum5}};
    auto token2 = vnd::Token{IDENTIFIER, "d", vnd::CodeSourceLocation{filename, t_line, t_colum6}};
    auto token3 = vnd::Token{IDENTIFIER, "a", vnd::CodeSourceLocation{filename, t_line, t_colum4}};
    auto token4 = vnd::Token{STAR, "*", vnd::CodeSourceLocation{filename, t_line, t_colum5}};
    auto token5 = vnd::Token{IDENTIFIER, "s", vnd::CodeSourceLocation{filename, t_line, t_colum6}};
    auto token6 = vnd::Token{IDENTIFIER, "b", vnd::CodeSourceLocation{filename, t_line, t_colum5}};
    vnd::BinaryExpressionNode unara{"-", token1, MAKE_UNIQUE(vnd::VariableNode, "d", token2), MAKE_UNIQUE(vnd::VariableNode, "a", token3)};
    vnd::BinaryExpressionNode unarb{"*", token4, MAKE_UNIQUE(vnd::VariableNode, "s", token5), MAKE_UNIQUE(vnd::VariableNode, "b", token6)};
    REQUIRE(unara.getOp() == "-");
    REQUIRE(unara.getLeft()->as<vnd::VariableNode>()->getName() == "d");
    REQUIRE(unara.getRight()->as<vnd::VariableNode>()->getName() == "a");
    REQUIRE(unara.get_token() == token1);
    REQUIRE(unara.getLeft()->as<vnd::VariableNode>()->get_token() == token2);
    REQUIRE(unara.getRight()->as<vnd::VariableNode>()->get_token() == token3);
    REQUIRE(unarb.getOp() == "*");
    REQUIRE(unarb.getLeft()->as<vnd::VariableNode>()->getName() == "s");
    REQUIRE(unarb.getRight()->as<vnd::VariableNode>()->getName() == "b");
    REQUIRE(unarb.get_token() == token4);
    REQUIRE(unarb.getLeft()->as<vnd::VariableNode>()->get_token() == token5);
    REQUIRE(unarb.getRight()->as<vnd::VariableNode>()->get_token() == token6);
    std::swap(unara, unarb);
    REQUIRE(unara.getOp() == "*");
    REQUIRE(unara.getLeft()->as<vnd::VariableNode>()->getName() == "s");
    REQUIRE(unara.getRight()->as<vnd::VariableNode>()->getName() == "b");
    REQUIRE(unara.get_token() == token4);
    REQUIRE(unara.getLeft()->as<vnd::VariableNode>()->get_token() == token5);
    REQUIRE(unara.getRight()->as<vnd::VariableNode>()->get_token() == token6);
    REQUIRE(unarb.getOp() == "-");
    REQUIRE(unarb.getLeft()->as<vnd::VariableNode>()->getName() == "d");
    REQUIRE(unarb.getRight()->as<vnd::VariableNode>()->getName() == "a");
    REQUIRE(unarb.get_token() == token1);
    REQUIRE(unarb.getLeft()->as<vnd::VariableNode>()->get_token() == token2);
    REQUIRE(unarb.getRight()->as<vnd::VariableNode>()->get_token() == token3);
}

TEST_CASE("Parser::convertToInt tests", "[parser]") {
    SECTION("Valid integer conversion") {
        REQUIRE(vnd::Parser::convertToInt("123") == 123);
        REQUIRE(vnd::Parser::convertToInt("-456") == -456);
    }

    SECTION("Invalid argument throws exception") {
        REQUIRE_THROWS_MATCHES(vnd::Parser::convertToInt("abc"), std::invalid_argument, Message(pIntInvArg));
    }

    SECTION("Result out of range throws exception") {
        // Assuming int is 32-bit
        REQUIRE_THROWS_MATCHES(vnd::Parser::convertToInt("2147483648"), std::out_of_range, Message(pIntOutRng));
    }

    SECTION("Trailing characters throws exception") {
        REQUIRE_THROWS_MATCHES(vnd::Parser::convertToInt("123a"), std::invalid_argument, Message(pIntTrlChr));
    }
}

TEST_CASE("Parser::convertToIntformExa tests") {
    SECTION("Valid hexadecimal conversion") {
        REQUIRE(vnd::Parser::convertToIntformExa("1A") == 26);     // 0x1A = 26 in decimal
        REQUIRE(vnd::Parser::convertToIntformExa("-FF") == -255);  // 0xFF = -255 in decimal
    }

    SECTION("Invalid argument throws exception") {
        REQUIRE_THROWS_MATCHES(vnd::Parser::convertToIntformExa("xyz"), std::invalid_argument, Message(pIntInvArg));
    }

    SECTION("Result out of range throws exception") {
        // Assuming int is 32-bit
        REQUIRE_THROWS_MATCHES(vnd::Parser::convertToIntformExa("80000000"), std::out_of_range, Message(pIntOutRng));
    }

    SECTION("Trailing characters throws exception") {
        REQUIRE_THROWS_MATCHES(vnd::Parser::convertToIntformExa("1A a"), std::invalid_argument, Message(pIntTrlChr));
    }
}

TEST_CASE("Parser::convertToIntformOct tests") {
    SECTION("Valid octal conversion") {
        REQUIRE(vnd::Parser::convertToIntformOct("77") == 63);   // 077 = 63 in decimal
        REQUIRE(vnd::Parser::convertToIntformOct("-10") == -8);  // -010 = -8 in decimal
    }

    SECTION("Invalid argument throws exception") {
        REQUIRE_THROWS_MATCHES(vnd::Parser::convertToIntformOct("xyz"), std::invalid_argument, Message(pIntInvArg));
    }

    SECTION("Result out of range throws exception") {
        // Assuming int is 32-bit
        REQUIRE_THROWS_MATCHES(vnd::Parser::convertToIntformOct("20000000000"), std::out_of_range, Message(pIntOutRng));
    }

    SECTION("Trailing characters throws exception") {
        REQUIRE_THROWS_MATCHES(vnd::Parser::convertToIntformOct("77a"), std::invalid_argument, Message(pIntTrlChr));
    }
}

TEST_CASE("comp_NodeType function", "[parser]") {
    REQUIRE(std::string(comp_NodeType(NodeType::BinaryExpression)) == "BIN_EXP");
    REQUIRE(std::string(comp_NodeType(NodeType::UnaryExpression)) == "UN_EXP");
    REQUIRE(std::string(comp_NodeType(NodeType::Number)) == "NUM");
    REQUIRE(std::string(comp_NodeType(NodeType::Boolean)) == "BOOL");
    REQUIRE(std::string(comp_NodeType(NodeType::Char)) == "CHAR");
    REQUIRE(std::string(comp_NodeType(NodeType::String)) == "STR");
    REQUIRE(std::string(comp_NodeType(NodeType::Variable)) == "VAR");
    REQUIRE(std::string(comp_NodeType(NodeType::Nullptr)) == "NULL");
    REQUIRE(std::string(comp_NodeType(NodeType::Type)) == "TYPE");
    REQUIRE(std::string(comp_NodeType(NodeType::Index)) == "IDX");
    REQUIRE(std::string(comp_NodeType(NodeType::Array)) == "ARR");
}

TEST_CASE("Parser emit integer number node form exadecimal", "[parser]") {
    vnd::Parser parser("#23", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<VND_NUM_INT>();
    REQUIRE(number != nullptr);
#ifdef _MSC_VER
    REQUIRE(number->getTypeIDName() == "int");
#else
    REQUIRE(number->getTypeIDName() == "i");
#endif
    REQUIRE(number->get_value() == 35);
}

TEST_CASE("Parser emit integer number node form exadecimal max int -1", "[parser]") {
    vnd::Parser parser("#7ffffffe", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<VND_NUM_INT>();
    REQUIRE(number != nullptr);
#ifdef _MSC_VER
    REQUIRE(number->getTypeIDName() == "int");
#else
    REQUIRE(number->getTypeIDName() == "i");
#endif
    REQUIRE(number->get_value() == 2147483646);
}

TEST_CASE("Parser emit integer number node form octal", "[parser]") {
    vnd::Parser parser("#o23", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<VND_NUM_INT>();
    REQUIRE(number != nullptr);
#ifdef _MSC_VER
    REQUIRE(number->getTypeIDName() == "int");
#else
    REQUIRE(number->getTypeIDName() == "i");
#endif
    REQUIRE(number->get_value() == 19);
}

TEST_CASE("Parser emit integer number node form octal max int -1", "[parser]") {
    vnd::Parser parser("#o17777777776", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<VND_NUM_INT>();
    REQUIRE(number != nullptr);
#ifdef _MSC_VER
    REQUIRE(number->getTypeIDName() == "int");
#else
    REQUIRE(number->getTypeIDName() == "i");
#endif
    REQUIRE(number->get_value() == 2147483646);
}

TEST_CASE("Parser emit integer number node print", "[parser]") {
    vnd::Parser parser("1", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<VND_NUM_INT>();
    REQUIRE(number != nullptr);
    REQUIRE(number->get_value() == 1);
#ifdef _MSC_VER
    REQUIRE(number->getTypeIDName() == "int");
#else
    REQUIRE(number->getTypeIDName() == "i");
#endif
    REQUIRE(number->print() == "NUMBER_INTEGER(1)");
}

TEST_CASE("Parser emit integer number node compat print", "[parser]") {
    vnd::Parser parser("1", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<VND_NUM_INT>();
    REQUIRE(number != nullptr);
    REQUIRE(number->get_value() == 1);
#ifdef _MSC_VER
    REQUIRE(number->getTypeIDName() == "int");
#else
    REQUIRE(number->getTypeIDName() == "i");
#endif
    REQUIRE(number->comp_print() == "NUM_INT(1)");
}

TEST_CASE("Parser emit variable node", "[parser]") {
    vnd::Parser parser("y", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Variable);
    const auto *variable = ast->as<vnd::VariableNode>();
    REQUIRE(variable != nullptr);
    REQUIRE(variable->getName() == "y");
}

TEST_CASE("Parser emit double number node double", "[parser]") {
    vnd::Parser parser("1.5", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<VND_NUM_DOUBLE>();
    REQUIRE(number != nullptr);
#ifdef _MSC_VER
    REQUIRE(number->getTypeIDName() == "double");
#else
    REQUIRE(number->getTypeIDName() == "d");
#endif
    REQUIRE(number->get_value() == 1.5);
}

TEST_CASE("Parser emit flaot number node float", "[parser]") {
    vnd::Parser parser("1.5f", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<VND_NUM_FLOAT>();
    REQUIRE(number != nullptr);
#ifdef _MSC_VER
    REQUIRE(number->getTypeIDName() == "float");
#else
    REQUIRE(number->getTypeIDName() == "f");
#endif
    REQUIRE(number->get_value() == 1.5F);
}

TEST_CASE("Parser emit double number node double print", "[parser]") {
    vnd::Parser parser("1.5", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<VND_NUM_DOUBLE>();
    REQUIRE(number != nullptr);
#ifdef _MSC_VER
    REQUIRE(number->getTypeIDName() == "double");
#else
    REQUIRE(number->getTypeIDName() == "d");
#endif
    REQUIRE(number->get_value() == 1.5);
    REQUIRE(number->print() == "NUMBER_DOUBLE(1.5)");
}

TEST_CASE("Parser emit flaot number node float print", "[parser]") {
    vnd::Parser parser("1.5f", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<VND_NUM_FLOAT>();
    REQUIRE(number != nullptr);
#ifdef _MSC_VER
    REQUIRE(number->getTypeIDName() == "float");
#else
    REQUIRE(number->getTypeIDName() == "f");
#endif
    REQUIRE(number->get_value() == 1.5F);
    REQUIRE(number->print() == "NUMBER_FLOAT(1.5)");
}

TEST_CASE("Parser emit double number node double compat print", "[parser]") {
    vnd::Parser parser("1.5", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<VND_NUM_DOUBLE>();
    REQUIRE(number != nullptr);
#ifdef _MSC_VER
    REQUIRE(number->getTypeIDName() == "double");
#else
    REQUIRE(number->getTypeIDName() == "d");
#endif
    REQUIRE(number->get_value() == 1.5);
    REQUIRE(number->comp_print() == "NUM_DBL(1.5)");
}

TEST_CASE("Parser emit flaot number node float compat print", "[parser]") {
    vnd::Parser parser("1.5f", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Number);
    const auto *number = ast->as<VND_NUM_FLOAT>();
    REQUIRE(number != nullptr);
#ifdef _MSC_VER
    REQUIRE(number->getTypeIDName() == "float");
#else
    REQUIRE(number->getTypeIDName() == "f");
#endif
    REQUIRE(number->get_value() == 1.5F);
    REQUIRE(number->comp_print() == "NUM_FLT(1.5)");
}

TEST_CASE("Parser emit variable node print", "[parser]") {
    vnd::Parser parser("y", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Variable);
    const auto *variable = ast->as<vnd::VariableNode>();
    REQUIRE(variable != nullptr);
    REQUIRE(variable->getName() == "y");
    REQUIRE(variable->print() == "VARIABLE(y)");
}

TEST_CASE("Parser emit variable node compat print", "[parser]") {
    vnd::Parser parser("y", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Variable);
    const auto *variable = ast->as<vnd::VariableNode>();
    REQUIRE(variable != nullptr);
    REQUIRE(variable->getName() == "y");
    REQUIRE(variable->comp_print() == "VAR(y)");
}

TEST_CASE("Parser emit unary expression node", "[parser]") {
    vnd::Parser parser("-x", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::UnaryExpression);

    const auto *unaryNode = ast->as<vnd::UnaryExpressionNode>();

    // Check the operator and operand
    REQUIRE(unaryNode->getOp() == "-");

    const auto &operand = unaryNode->getOperand();
    REQUIRE(operand != nullptr);
    REQUIRE(operand->getType() == NodeType::Variable);

    const auto *variableNode = operand->as<vnd::VariableNode>();
    REQUIRE(variableNode != nullptr);
    REQUIRE(variableNode->get_parent() != nullptr);
    REQUIRE(variableNode->getName() == "x");
}

TEST_CASE("Parser emit unary expression node print", "[parser]") {
    vnd::Parser parser("-x", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::UnaryExpression);

    const auto *unaryNode = ast->as<vnd::UnaryExpressionNode>();

    REQUIRE(unaryNode->getOp() == "-");

    const auto &operand = unaryNode->getOperand();
    REQUIRE(operand != nullptr);
    REQUIRE(operand->getType() == NodeType::Variable);

    const auto *variableNode = operand->as<vnd::VariableNode>();
    REQUIRE(variableNode != nullptr);
    REQUIRE(variableNode->get_parent() != nullptr);
    REQUIRE(variableNode->getName() == "x");
    REQUIRE(unaryNode->print() == "UNARY_EXPRESION(op:\"-\" operand:VARIABLE(x))");
}

TEST_CASE("Parser emit unary expression node compat print", "[parser]") {
    vnd::Parser parser("-x", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::UnaryExpression);

    const auto *unaryNode = ast->as<vnd::UnaryExpressionNode>();

    REQUIRE(unaryNode->getOp() == "-");

    const auto &operand = unaryNode->getOperand();
    REQUIRE(operand != nullptr);
    REQUIRE(operand->getType() == NodeType::Variable);

    const auto *variableNode = operand->as<vnd::VariableNode>();
    REQUIRE(variableNode != nullptr);
    REQUIRE(variableNode->get_parent() != nullptr);
    REQUIRE(variableNode->getName() == "x");
    REQUIRE(unaryNode->comp_print() == "UNE(op:\"-\" opr:VAR(x))");
}

TEST_CASE("Parser emit binary expression node", "[parser]") {
    vnd::Parser parser("1 + 2", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::BinaryExpression);

    const auto *binaryNode = ast->as<vnd::BinaryExpressionNode>();
    REQUIRE(binaryNode != nullptr);

    REQUIRE(binaryNode->getOp() == "+");

    const auto *leftNumber = binaryNode->getLeft()->as<VND_NUM_INT>();
    const auto *rightNumber = binaryNode->getRight()->as<VND_NUM_INT>();

    REQUIRE(leftNumber != nullptr);
    REQUIRE(rightNumber != nullptr);
    REQUIRE(leftNumber->get_parent() != nullptr);
    REQUIRE(rightNumber->get_parent() != nullptr);
#ifdef _MSC_VER
    REQUIRE(leftNumber->getTypeIDName() == "int");
    REQUIRE(rightNumber->getTypeIDName() == "int");
#else
    REQUIRE(leftNumber->getTypeIDName() == "i");
    REQUIRE(rightNumber->getTypeIDName() == "i");
#endif

    REQUIRE(leftNumber->get_value() == 1);
    REQUIRE(rightNumber->get_value() == 2);
}

TEST_CASE("Parser emit binary expression node print", "[parser]") {
    vnd::Parser parser("1 + 2", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::BinaryExpression);

    const auto *binaryNode = ast->as<vnd::BinaryExpressionNode>();

    REQUIRE(binaryNode->getOp() == "+");

    const auto *leftNumber = binaryNode->getLeft()->as<VND_NUM_INT>();
    const auto *rightNumber = binaryNode->getRight()->as<VND_NUM_INT>();

    REQUIRE(leftNumber != nullptr);
    REQUIRE(rightNumber != nullptr);
    REQUIRE(leftNumber->get_parent() != nullptr);
    REQUIRE(rightNumber->get_parent() != nullptr);
#ifdef _MSC_VER
    REQUIRE(leftNumber->getTypeIDName() == "int");
    REQUIRE(rightNumber->getTypeIDName() == "int");
#else
    REQUIRE(leftNumber->getTypeIDName() == "i");
    REQUIRE(rightNumber->getTypeIDName() == "i");
#endif

    REQUIRE(leftNumber->get_value() == 1);
    REQUIRE(rightNumber->get_value() == 2);
    REQUIRE(binaryNode->print() == R"(BINARY_EXPRESION(op:"+" left:NUMBER_INTEGER(1), right:NUMBER_INTEGER(2)))");
}

TEST_CASE("Parser emit binary expression node compact print", "[parser]") {
    vnd::Parser parser("1 + 2", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::BinaryExpression);

    const auto *binaryNode = ast->as<vnd::BinaryExpressionNode>();

    REQUIRE(binaryNode->getOp() == "+");

    const auto *leftNumber = binaryNode->getLeft()->as<VND_NUM_INT>();
    const auto *rightNumber = binaryNode->getRight()->as<VND_NUM_INT>();

    REQUIRE(leftNumber != nullptr);
    REQUIRE(rightNumber != nullptr);
    REQUIRE(leftNumber->get_parent() != nullptr);
    REQUIRE(rightNumber->get_parent() != nullptr);
#ifdef _MSC_VER
    REQUIRE(leftNumber->getTypeIDName() == "int");
    REQUIRE(rightNumber->getTypeIDName() == "int");
#else
    REQUIRE(leftNumber->getTypeIDName() == "i");
    REQUIRE(rightNumber->getTypeIDName() == "i");
#endif

    REQUIRE(leftNumber->get_value() == 1);
    REQUIRE(rightNumber->get_value() == 2);
    REQUIRE(binaryNode->comp_print() == R"(BINE(op:"+" l:NUM_INT(1), r:NUM_INT(2)))");
}

TEST_CASE("Parser pars complex expression", "[parser]") {
    // clang-format off
    vnd::Parser parser{"2 + 3 + (4.2 / 2) * 3 + y", "input.vn"};
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->print() == R"(BINARY_EXPRESION(op:"+" left:BINARY_EXPRESION(op:"+" left:BINARY_EXPRESION(op:"+" left:NUMBER_INTEGER(2), right:NUMBER_INTEGER(3)), right:BINARY_EXPRESION(op:"*" left:BINARY_EXPRESION(op:"/" left:NUMBER_DOUBLE(4.2), right:NUMBER_INTEGER(2)), right:NUMBER_INTEGER(3))), right:VARIABLE(y)))");
    REQUIRE(ast->comp_print() == R"(BINE(op:"+" l:BINE(op:"+" l:BINE(op:"+" l:NUM_INT(2), r:NUM_INT(3)), r:BINE(op:"*" l:BINE(op:"/" l:NUM_DBL(4.2), r:NUM_INT(2)), r:NUM_INT(3))), r:VAR(y)))");
    // clang-format on
}

TEST_CASE("Parser emit exception for mismacted  paren", "[parser]") {
    vnd::Parser tokenizer{"1 + 2 +( 2+3*3", filename};
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    REQUIRE_THROWS_MATCHES(
        tokenizer.parse(), vnd::ParserException,
        Message(R"(Unexpected token: (type: OPEN_PARENTESIS, value: '(', source location:(file:.\unknown.vn, line:1, column:8)))"));
#else
    REQUIRE_THROWS_MATCHES(
        tokenizer.parse(), vnd::ParserException,
        Message(R"(Unexpected token: (type: OPEN_PARENTESIS, value: '(', source location:(file:./unknown.vn, line:1, column:8)))"));

#endif
}

TEST_CASE("Parser emit exception for uncomplete expression", "[parser]") {
    vnd::Parser tokenizer{"1 + 2 *", filename};
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    REQUIRE_THROWS_MATCHES(tokenizer.parse(), vnd::ParserException,
                           Message("Unexpected token: (type: STAR_OPERATOR, value: '*', "
                                   "source location:(file:.\\unknown.vn, line:1, column:7))"));
#else
    REQUIRE_THROWS_MATCHES(tokenizer.parse(), vnd::ParserException,
                           Message("Unexpected token: (type: STAR_OPERATOR, value: '*', "
                                   "source location:(file:./unknown.vn, line:1, column:7))"));
#endif
}

TEST_CASE("Parser emit exception for nonexistent unary operator", "[parser]") {
    vnd::Parser tokenizer{"*2", filename};
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    REQUIRE_THROWS_MATCHES(
        tokenizer.parse(), vnd::ParserException,
        Message(R"(Unexpected token: (type: STAR_OPERATOR, value: '*', source location:(file:.\unknown.vn, line:1, column:1)))"));
#else
    REQUIRE_THROWS_MATCHES(
        tokenizer.parse(), vnd::ParserException,
        Message(R"(Unexpected token: (type: STAR_OPERATOR, value: '*', source location:(file:./unknown.vn, line:1, column:1)))"));
#endif
}

TEST_CASE("NodeType formatter works as expected", "[formatter]") {
    using enum NodeType;  // shorthand for enum access
    REQ_FORMAT(BinaryExpression, "BINARY_EXPRESION");
    REQ_FORMAT(UnaryExpression, "UNARY_EXPRESION");
    REQ_FORMAT(Number, "NUMBER");
    REQ_FORMAT(Boolean, "BOOLEAN");
    REQ_FORMAT(Char, "CHAR");
    REQ_FORMAT(String, "STRING");
    REQ_FORMAT(Variable, "VARIABLE");
    REQ_FORMAT(Nullptr, "NULLPTR");
    REQ_FORMAT(Type, "TYPE");
    REQ_FORMAT(Index, "INDEX");
    REQ_FORMAT(Array, "ARRAY");
    REQ_FORMAT(Statement, "STATEMENT");
}

TEST_CASE("Parser emit i8 TypeNode node", "[parser]") {
    vnd::Parser parser("i8", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Type);
    REQUIRE(ast->get_parent() == nullptr);
    const auto *typeNode = ast->as<vnd::TypeNode>();
    REQUIRE(typeNode != nullptr);
    REQUIRE(typeNode->comp_print() == "TYPE(i8)");
    REQUIRE(typeNode->print() == "TYPE_I8_TYPE(i8)");
    REQUIRE(typeNode->getVariableType() == vnd::TokenType::TYPE_I8);
    REQUIRE(typeNode->get_value() == "i8");
    REQUIRE(typeNode->get_index() == nullptr);
}

TEST_CASE("Parser emit array", "[parser]") {
    vnd::Parser parser("i8[size][]{1, 2}", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Type);
    REQUIRE(ast->get_parent() == nullptr);
    const auto *typeNode = ast->as<vnd::TypeNode>();
    REQUIRE(typeNode != nullptr);
    REQUIRE(typeNode->get_index() != nullptr);
    REQUIRE(typeNode->get_index()->getType() == NodeType::Index);
    REQUIRE(typeNode->get_index()->get_elements() != nullptr);
    REQUIRE(typeNode->get_index()->get_index() != nullptr);
    REQUIRE(typeNode->get_index()->get_array() == nullptr);
    REQUIRE(typeNode->get_index()->get_index()->get_elements() == nullptr);
    REQUIRE(typeNode->get_index()->get_index()->get_index() == nullptr);
    REQUIRE(typeNode->get_index()->get_index()->get_array() != nullptr);
    REQUIRE(typeNode->get_index()->get_index()->get_array()->getType() == NodeType::Array);
    REQUIRE(typeNode->get_index()->get_index()->get_array()->get_elements() != nullptr);
}

TEST_CASE("Parser emit empty array", "[parser]") {
    vnd::Parser parser("Object[]{}", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Variable);
    REQUIRE(ast->get_parent() == nullptr);
    const auto *variable = ast->as<vnd::VariableNode>();
    REQUIRE(variable != nullptr);
    REQUIRE(variable->get_index() != nullptr);
    REQUIRE(variable->get_index()->get_array() != nullptr);
    REQUIRE(variable->get_index()->get_array()->getType() == NodeType::Array);
    REQUIRE(variable->get_index()->get_array()->get_elements() == nullptr);
}

TEST_CASE("Parser emit array type", "[parser]") {
    vnd::Parser parser("i8[]", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Type);
    REQUIRE(ast->get_parent() == nullptr);
    const auto *typeNode = ast->as<vnd::TypeNode>();
    REQUIRE(typeNode != nullptr);
    REQUIRE(typeNode->get_index() != nullptr);
    REQUIRE(typeNode->get_index()->getType() == NodeType::Index);
    REQUIRE(typeNode->get_index()->get_elements() == nullptr);
    REQUIRE(typeNode->get_index()->get_index() == nullptr);
    REQUIRE(typeNode->get_index()->get_array() == nullptr);
}

TEST_CASE("Parser emit nullptr node", "[parser]") {
    vnd::Parser parser("nullptr", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Nullptr);
    REQUIRE(ast->get_parent() == nullptr);
    const auto *nullptrNode = ast->as<vnd::NullptrNode>();
    REQUIRE(nullptrNode != nullptr);
}

TEST_CASE("Parser emit exception on comment", "[parser]") {
    vnd::Parser parser("// comment", filename);
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    REQUIRE_THROWS_MATCHES(
        parser.parse(), vnd::ParserException,
        Message(R"(Unexpected token: (type: COMMENT, value: '// comment', source location:(file:.\unknown.vn, line:1, column:1)))"));
#else
    REQUIRE_THROWS_MATCHES(
        parser.parse(), vnd::ParserException,
        Message(R"(Unexpected token: (type: COMMENT, value: '// comment', source location:(file:./unknown.vn, line:1, column:1)))"));
#endif
}

TEST_CASE("Parser emit exception on multiline comment", "[parser]") {
    vnd::Parser parser(R"(/*multi\nline\ncomment*/)", filename);
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    REQUIRE_THROWS_MATCHES(
        parser.parse(), vnd::ParserException,
        Message(
            R"(Unexpected token: (type: COMMENT, value: '/*multi\nline\ncomment*/', source location:(file:.\unknown.vn, line:1, column:1)))"));
#else
    REQUIRE_THROWS_MATCHES(
        parser.parse(), vnd::ParserException,
        Message(
            R"(Unexpected token: (type: COMMENT, value: '/*multi\nline\ncomment*/', source location:(file:./unknown.vn, line:1, column:1)))"));
#endif
}

/* TEST_CASE("Parser emit mismatched square brackets exception", "[parser]") {
    vnd::Parser parser("Object[size", filename);
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    REQUIRE_THROWS_MATCHES(parser.parse(), vnd::ParserException,
                           Message(R"(Unexpected token: (type: EOF, source location:(file:.\unknown.vn, line:1, column:12)))"));
#else
    REQUIRE_THROWS_MATCHES(parser.parse(), vnd::ParserException,
                           Message(R"(Unexpected token: (type: EOF, source location:(file:./unknown.vn, line:1, column:12)))"));
#endif
}

TEST_CASE("Parser emit mismatched curly brackets exception", "[parser]") {
    vnd::Parser parser("string[size]{", filename);
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    REQUIRE_THROWS_MATCHES(parser.parse(), vnd::ParserException,
                           Message(R"(Unexpected token: (type: EOF, source location:(file:.\unknown.vn, line:1, column:14)))"));
#else
    REQUIRE_THROWS_MATCHES(parser.parse(), vnd::ParserException,
                           Message(R"(Unexpected token: (type: EOF, source location:(file:./unknown.vn, line:1, column:14)))"));
#endif
}*/
TEST_CASE("Parser emit empty index node print", "[parser]") {
    vnd::Parser parser("i8[]", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);

    REQUIRE(ast->getType() == NodeType::Type);
    REQUIRE(ast->get_parent() == nullptr);
    const auto *typeNode = ast->as<vnd::TypeNode>();
    REQUIRE(typeNode != nullptr);
    REQUIRE(typeNode->get_index()->print() == "INDEX()");
}

TEST_CASE("Parser emit empty index compat print", "[parser]") {
    vnd::Parser parser("i8[]", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Type);
    REQUIRE(ast->get_parent() == nullptr);
    const auto *typeNode = ast->as<vnd::TypeNode>();
    REQUIRE(typeNode != nullptr);
    REQUIRE(typeNode->get_index()->comp_print() == FORMAT("INDEX"));
}

TEST_CASE("Parser emit index node print", "[parser]") {
    vnd::Parser parser("i8[1]", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Type);
    REQUIRE(ast->get_parent() == nullptr);
    const auto *typeNode = ast->as<vnd::TypeNode>();
    REQUIRE(typeNode != nullptr);
    REQUIRE(typeNode->get_index()->print() == FORMAT("INDEX({})", typeNode->get_index()->get_elements()->comp_print()));
}

TEST_CASE("Parser emit index compat print", "[parser]") {
    vnd::Parser parser("i8[1]", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Type);
    REQUIRE(ast->get_parent() == nullptr);
    const auto *typeNode = ast->as<vnd::TypeNode>();
    REQUIRE(typeNode != nullptr);
    REQUIRE(typeNode->get_index()->comp_print() == FORMAT("INDEX"));
}

/*TEST_CASE("Parser emit empty array node print", "[parser]") {
    vnd::Parser parser("{}", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Array);
    REQUIRE(ast->get_parent() == nullptr);
    const auto *arrayNode = ast->as<vnd::ArrayNode>();
    REQUIRE(arrayNode != nullptr);
    REQUIRE(arrayNode->print() == "ARRAY()");
}

TEST_CASE("Parser emit empty array compat node print", "[parser]") {
    vnd::Parser parser("{}", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Array);
    REQUIRE(ast->get_parent() == nullptr);
    const auto *arrayNode = ast->as<vnd::ArrayNode>();
    REQUIRE(arrayNode != nullptr);
    REQUIRE(arrayNode->comp_print() == "ARRAY");
}

TEST_CASE("Parser emit array node print", "[parser]") {
    vnd::Parser parser("{1, 2}", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Array);
    REQUIRE(ast->get_parent() == nullptr);
    const auto *arrayNode = ast->as<vnd::ArrayNode>();
    REQUIRE(arrayNode != nullptr);
    REQUIRE(arrayNode->print() == FORMAT("ARRAY({})", arrayNode->get_elements()->comp_print()));
}

TEST_CASE("Parser emit array compat node print", "[parser]") {
    vnd::Parser parser("{1, 2}", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Array);
    REQUIRE(ast->get_parent() == nullptr);
    const auto *arrayNode = ast->as<vnd::ArrayNode>();
    REQUIRE(arrayNode != nullptr);
    REQUIRE(arrayNode->comp_print() == "ARRAY");
}*/

TEST_CASE("Parser emit empty callable node", "[parser]") {
    vnd::Parser parser("function()", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Variable);
    REQUIRE(ast->get_parent() == nullptr);
    const auto *variableNode = ast->as<vnd::VariableNode>();
    REQUIRE(variableNode->is_call() == true);
    REQUIRE(variableNode->get_call() == nullptr);
}

TEST_CASE("Parser emit callable node", "[parser]") {
    vnd::Parser parser("function(1)", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE(ast->getType() == NodeType::Variable);
    const auto *variableNode = ast->as<vnd::VariableNode>();
    REQUIRE(variableNode->is_call() == true);
    REQUIRE(variableNode->get_call() != nullptr);
}

TEST_CASE("NullptrNode basic functionality", "[NullptrNode]") {
    vnd::Parser parser("nullptr", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    SECTION("Constructor Test") {
        auto node = ast->safe_as<vnd::NullptrNode>();
        REQUIRE(node->get_token().getType() == vnd::TokenType::K_NULLPTR);
    }

    SECTION("getType() Test") {
        auto node = ast->safe_as<vnd::NullptrNode>();
        REQUIRE(node->getType() == NodeType::Nullptr);
    }

    SECTION("print() Test") {
        auto node = ast->safe_as<vnd::NullptrNode>();
        REQUIRE(node->print() == "NULLPTR");
    }

    SECTION("comp_print() Test") {
        auto node = ast->safe_as<vnd::NullptrNode>();
        REQUIRE(node->comp_print() == "NULL");
    }
}

TEST_CASE("GetBuildFolder - Standard Cases") {
    SECTION("Normal path without trailing slash") {
        const fs::path inputPath = fs::path("home/user/project").make_preferred();
        const fs::path expectedOutput = fs::path("home/user/vnbuild").make_preferred();
        REQUIRE(vnd::GetBuildFolder(inputPath) == expectedOutput);
    }

    SECTION("Path with trailing slash") {
        const fs::path inputPath = fs::path("home/user/project/").make_preferred();
        const fs::path expectedOutput = fs::path("home/user/vnbuild").make_preferred();
        REQUIRE(vnd::GetBuildFolder(inputPath) == expectedOutput);
    }

    SECTION("Nested directory structure") {
        const fs::path inputPath = fs::path("home/user/projects/client/app").make_preferred();
        const fs::path expectedOutput = fs::path("home/user/projects/client/vnbuild").make_preferred();
        REQUIRE(vnd::GetBuildFolder(inputPath) == expectedOutput);
    }
}

TEST_CASE("GetBuildFolder - Edge Cases") {
    SECTION("Root directory input") {
        const fs::path inputPath = fs::path("/").make_preferred();
        const fs::path expectedOutput = fs::path("/vnbuild").make_preferred();
        REQUIRE(vnd::GetBuildFolder(inputPath) == expectedOutput);
    }

    SECTION("Empty path") {
        const fs::path inputPath = fs::path("").make_preferred();
        const fs::path expectedOutput = fs::path("vnbuild").make_preferred();  // No parent; expects vnbuild in current directory
        REQUIRE(vnd::GetBuildFolder(inputPath) == expectedOutput);
    }

    SECTION("Relative path") {
        const fs::path inputPath = fs::path("folder/subfolder").make_preferred();
        const fs::path expectedOutput = fs::path("folder/vnbuild").make_preferred();
        REQUIRE(vnd::GetBuildFolder(inputPath) == expectedOutput);
    }

    SECTION("Single directory path") {
        const fs::path inputPath = fs::path("parent").make_preferred();
        const fs::path expectedOutput = fs::path("vnbuild").make_preferred();
        REQUIRE(vnd::GetBuildFolder(inputPath) == expectedOutput);
    }

    SECTION("Current directory input") {
        const fs::path inputPath = fs::path(".").make_preferred();
        const fs::path expectedOutput = fs::path("vnbuild").make_preferred();
        REQUIRE(vnd::GetBuildFolder(inputPath) == expectedOutput);
    }

    SECTION("Parent directory input") {
        const fs::path inputPath = fs::path("..").make_preferred();
        const fs::path expectedOutput = fs::path("../vnbuild").make_preferred();
        REQUIRE(vnd::GetBuildFolder(inputPath) == expectedOutput);
    }

    SECTION("Path with special characters") {
        const fs::path inputPath = fs::path("/path/with special@chars!").make_preferred();
        const fs::path expectedOutput = fs::path("/path/vnbuild").make_preferred();
        REQUIRE(vnd::GetBuildFolder(inputPath) == expectedOutput);
    }
}

TEST_CASE("Transpiler creates correct folders and files c++", "[transpiler]") {
    const std::string transpilerfilename = "testfile.vnd";

    vnd::Transpiler transpiler(long_input, transpilerfilename);

    SECTION("Folder and file creation") {
        transpiler.transpile();

        const fs::path buildFolder("vnbuild");
        const fs::path srcFolder = buildFolder / "src";
        REQUIRE(fs::exists(buildFolder));
        REQUIRE(fs::exists(srcFolder));

        const fs::path cppFile = srcFolder / "testfile.cpp";

        REQUIRE(fs::exists(cppFile));

        std::ifstream file(cppFile);
        std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        REQUIRE_THAT(fileContent, ContainsSubstring("Hello, World!"));  // Check for the presence of "Hello, World!"
        REQUIRE_THAT(fileContent, StartsWith("// This is an automatically generated file by Vandior"));
        REQUIRE_THAT(fileContent, EndsWith("return 0;\n}\n"));
    }

    SECTION("Clean up") {
        transpiler.transpile();

        const fs::path buildFolder("vnbuild");
        [[maybe_unused]] auto unused = fs::remove_all(buildFolder);
        REQUIRE_FALSE(fs::exists(buildFolder));  // Folder should not exist
    }
}

TEST_CASE("Transpiler creates correct folders and files c++ cmake", "[transpiler]") {
    const std::string transpilerfilename = "testfile.vnd";

    vnd::Transpiler transpiler(long_input, transpilerfilename, true);

    SECTION("Folder and file creation") {
        transpiler.transpile();

        const fs::path buildFolder("vnbuild");
        const fs::path srcFolder = buildFolder / "src";
        REQUIRE(fs::exists(buildFolder));
        REQUIRE(fs::exists(srcFolder));

        const fs::path cppFile = srcFolder / "testfile.cpp";
        const fs::path cmakeListsFile = buildFolder / "CMakeLists.txt";
        REQUIRE(fs::exists(cppFile));
        REQUIRE(fs::exists(cmakeListsFile));

        std::ifstream file(cppFile);
        std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        REQUIRE_THAT(fileContent, ContainsSubstring("Hello, World!"));  // Check for the presence of "Hello, World!"
        REQUIRE_THAT(fileContent, StartsWith("// This is an automatically generated file by Vandior"));
        REQUIRE_THAT(fileContent, EndsWith("return 0;\n}\n"));

        std::ifstream cmakeListsfile(cmakeListsFile);
        std::string cmakeListsfileContent((std::istreambuf_iterator<char>(cmakeListsfile)), std::istreambuf_iterator<char>());
        REQUIRE_THAT(cmakeListsfileContent, ContainsSubstring("# for more information got to  https://github.com/Giuseppe-Bianc/Vandior"));
        REQUIRE_THAT(cmakeListsfileContent, ContainsSubstring("set_target_properties(${PROJECT_NAME} PROPERTIES"));
        REQUIRE_THAT(cmakeListsfileContent, StartsWith("# This is an automatically generated file by Vandior"));
        REQUIRE_THAT(cmakeListsfileContent, EndsWith("endif()\n"));
    }

    SECTION("Clean up") {
        transpiler.transpile();

        const fs::path buildFolder("vnbuild");
        [[maybe_unused]] auto unused = fs::remove_all(buildFolder);
        REQUIRE_FALSE(fs::exists(buildFolder));  // Folder should not exist
    }
}

TEST_CASE("Transpiler::mapType returns correct type mappings", "[transpiler]") {
    SECTION("Valid type mappings") {
        REQUIRE(vnd::Transpiler::mapType("i8"sv) == "int8_t"sv);
        REQUIRE(vnd::Transpiler::mapType("i16"sv) == "int16_t"sv);
        REQUIRE(vnd::Transpiler::mapType("i32"sv) == "int32_t"sv);
        REQUIRE(vnd::Transpiler::mapType("i64"sv) == "int64_t"sv);
        REQUIRE(vnd::Transpiler::mapType("u8"sv) == "uint8_t"sv);
        REQUIRE(vnd::Transpiler::mapType("u16"sv) == "uint16_t"sv);
        REQUIRE(vnd::Transpiler::mapType("u32"sv) == "uint32_t"sv);
        REQUIRE(vnd::Transpiler::mapType("u64"sv) == "uint64_t"sv);
        REQUIRE(vnd::Transpiler::mapType("f32"sv) == "float"sv);
        REQUIRE(vnd::Transpiler::mapType("f64"sv) == "double"sv);
        REQUIRE(vnd::Transpiler::mapType("c32"sv) == "std::complex<float>"sv);
        REQUIRE(vnd::Transpiler::mapType("c64"sv) == "std::complex<double>"sv);
        REQUIRE(vnd::Transpiler::mapType("bool"sv) == "bool"sv);
        REQUIRE(vnd::Transpiler::mapType("char"sv) == "char"sv);
        REQUIRE(vnd::Transpiler::mapType("string"sv) == "std::string_view"sv);
    }

    SECTION("Unknown type returns 'unknown'") {
        REQUIRE(vnd::Transpiler::mapType("unknown_type"sv) == "unknown_type"sv);
        REQUIRE(vnd::Transpiler::mapType("xyz"sv) == "xyz"sv);
        REQUIRE(vnd::Transpiler::mapType(""sv).empty());
    }

    SECTION("Case sensitivity") {
        REQUIRE(vnd::Transpiler::mapType("I8"sv) == "I8"sv);  // Should be case sensitive
    }
}

TEST_CASE("prettyPrint: BinaryExpressionNode", "[prettyPrint]") {
    vnd::Parser parser("a + b", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);

    // Verificare che la funzione prettyPrint gestisca correttamente un BinaryExpressionNode
    REQUIRE_NOTHROW(prettyPrint(*ast, "", true));
}

TEST_CASE("prettyPrint: UnaryExpressionNode", "[prettyPrint]") {
    vnd::Parser parser("-b", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE_NOTHROW(prettyPrint(*ast, "", true));
}

TEST_CASE("prettyPrint: Variable", "[prettyPrint]") {
    vnd::Parser parser("b", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE_NOTHROW(prettyPrint(*ast, "", true));
}

TEST_CASE("prettyPrint: function call", "[prettyPrint]") {
    vnd::Parser parser("functiosss()", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE_NOTHROW(prettyPrint(*ast, "", true));
}

TEST_CASE("prettyPrint: function call2", "[prettyPrint]") {
    vnd::Parser parser("functiosss(2, 2)", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE_NOTHROW(prettyPrint(*ast, "", true));
}

TEST_CASE("prettyPrint: tipe assignment", "[prettyPrint]") {
    vnd::Parser parser("asd : i32", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE_NOTHROW(prettyPrint(*ast, "", true));
}

TEST_CASE("prettyPrint: array type assignment", "[prettyPrint]") {
    vnd::Parser parser("asd : i32[2], asd2 : i32[2][]", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE_NOTHROW(prettyPrint(*ast, "", true));
}

TEST_CASE("prettyPrint: array elements", "[prettyPrint]") {
    vnd::Parser parser("asd : i32[2] ={2, 3}", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE_NOTHROW(prettyPrint(*ast, "", true));
}

TEST_CASE("prettyPrint: base literals types", "[prettyPrint]") {
    vnd::Parser parser("12, 12.2f, 12.2, 12.2if, 12.2i, 'c', \"asdfgh\", true, false, nullptr", filename);
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE_NOTHROW(prettyPrint(*ast, "", true));
}

class UnknownNode : public vnd::ASTNode {
public:
    UnknownNode() : ASTNode(vnd::Token{}) {}
    [[nodiscard]] NodeType getType() const noexcept override { return NodeType::Unknown; }
    [[nodiscard]] std::string print() const override { return "UnknownNode"; }
    [[nodiscard]] std::string comp_print() const override { return "UnknownNode"; }
};
TEST_CASE("prettyPrint: Unknown node type", "[prettyPrint]") {
    // Creare un nodo fittizio non gestito

    const UnknownNode unknownNode;

    // Verificare che venga gestito correttamente
    REQUIRE_NOTHROW(prettyPrint(unknownNode));
}

TEST_CASE("printParentNode: no parent", "[printParentNode]") {
    vnd::Parser parser{"2 + 3 + (4.2 / 2) * 3 + y", "input.vn"};
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE_NOTHROW(printParentNode(*ast, ""));
}

// getLeft()

TEST_CASE("printParentNode: parent", "[printParentNode]") {
    vnd::Parser parser{"2 + 3 + (4.2 / 2) * 3 + y", "input.vn"};
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE_NOTHROW(printParentNode(*ast->as<vnd::BinaryExpressionNode>()->getLeft(), ""));
}

TEST_CASE("printParentNode: parent index", "[printParentNode]") {
    vnd::Parser parser{"asd[2][1]", "input.vn"};
    auto programAst = parser.parse();
    REQUIRE(programAst.size() == 1);
    auto ast = programAst[0].get_nodes().at(0).get();
    REQUIRE(ast != nullptr);
    REQUIRE_NOTHROW(printParentNode(*ast->as<vnd::VariableNode>()->get_index()->as<vnd::IndexNode>()->get_index(), ""));
}

TEST_CASE("vnd::timeParser", "[Vandior]") {
    vnd::Parser parser{"asdf", filename};
    std::vector<vnd::Statement> ast;
    vnd::timeParser(ast, parser);
    REQUIRE(ast.size() == 1);  // AST should be a valid object
}

TEST_CASE("vnd::timeParse", "[Vandior]") {
    vnd::Parser parser{"asdf", filename};

    auto ast = vnd::timeParse(parser);
    REQUIRE(ast.size() == 1);
}

TEST_CASE("Transpiler transpile main instruction", "[transpiler]") {
    vnd::Transpiler transpiler{"main {", "input.vn"};
    const auto code = transpiler.transpile();
    REQUIRE(code == "int main(int argc, char **argv) {\n");
}

TEST_CASE("Transpiler throw exception for missing {", "[transpiler]") {
    vnd::Transpiler transpiler{"main", "input.vn"};
    REQUIRE_THROWS_AS(transpiler.transpile(), vnd::ParserException);
}

TEST_CASE("Transpiler transpile i8 declaration instruction", "[transpiler]") {
    vnd::Transpiler transpiler{"var num1, num2: i8", "input.vn"};
    const auto code = transpiler.transpile();
    REQUIRE(code == "int8_t num1, num2\n");
}

TEST_CASE("Transpiler transpile declaration instruction", "[transpiler]") {
    vnd::Transpiler transpiler{"var var1, var2: type", "input.vn"};
    const auto code = transpiler.transpile();
    REQUIRE(code == "type var1, var2\n");
}

TEST_CASE("Transpiler transpile initialization instruction", "[transpiler]") {
    vnd::Transpiler transpiler{"var num1, num2: u8 = 12, 45", "input.vn"};
    const auto code = transpiler.transpile();
    REQUIRE(code == "uint8_t num1 = 12, num2  = 45\n");
}

TEST_CASE("Transpiler transpile vector initialization instruction", "[transpiler]") {
    vnd::Transpiler transpiler{"var nums: u8[] = u8[]{12, 45}", "input.vn"};
    const auto code = transpiler.transpile();
    REQUIRE(code == "vnd::vector<uint8_t> nums  = vnd::vector<uint8_t>({12, 45})\n");
}

TEST_CASE("Transpiler transpile structure instructions", "[transpiler]") {
    vnd::Transpiler transpiler{"if a == 1 {", "input.vn"};
    auto code = transpiler.transpile();
    REQUIRE(code == "if(a == 1) {\n");
    transpiler = vnd::Transpiler{"while a == 1 {", "input.vn"};
    code = transpiler.transpile();
    REQUIRE(code == "while(a == 1) {\n");
}

TEST_CASE("Transpiler transpile break and continue instructions", "[transpiler]") {
    vnd::Transpiler transpiler{"break", "input.vn"};
    auto code = transpiler.transpile();
    REQUIRE(code == "break\n");
    transpiler = vnd::Transpiler{"continue", "input.vn"};
    code = transpiler.transpile();
    REQUIRE(code == "continue\n");
}

TEST_CASE("Transpiler transpile void fun instruction", "[transpiler]") {
    vnd::Transpiler transpiler{"fun a() {", "input.vn"};
    const auto code = transpiler.transpile();
    REQUIRE(code == "auto a() -> void {\n");
}

TEST_CASE("Transpiler transpile single return value fun instruction", "[transpiler]") {
    vnd::Transpiler transpiler{"fun a(num1: i8, num2: type) i8 {", "input.vn"};
    const auto code = transpiler.transpile();
    REQUIRE(code == "auto a(int8_t num1, type num2) -> int8_t {\n");
}

TEST_CASE("Transpiler transpile multiple return value fun instruction", "[transpiler]") {
    vnd::Transpiler transpiler{"fun a() type, i8 {", "input.vn"};
    const auto code = transpiler.transpile();
    REQUIRE(code == "auto a() -> std::tuple< type , int8_t> {\n");
}

TEST_CASE("Transpiler transpile return instructions", "[transpiler]") {
    vnd::Transpiler transpiler{"return", "input.vn"};
    auto code = transpiler.transpile();
    REQUIRE(code == "return \n");
    transpiler = vnd::Transpiler{"return true", "input.vn"};
    code = transpiler.transpile();
    REQUIRE(code == "return true\n");
}
// clang-format off
// NOLINTEND(*-include-cleaner, *-avoid-magic-numbers, *-magic-numbers, *-unchecked-optional-access, *-avoid-do-while, *-use-anonymous-namespace, *-qualified-auto, *-suspicious-stringview-data-usage, *-err58-cpp, *-function-cognitive-complexity, *-macro-usage, *-unnecessary-copy-initialization)
// clang-format on