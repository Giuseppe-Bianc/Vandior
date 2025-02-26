function(check_simd_support RESULT_VAR FEATURE_NAME MSVC_FLAG OTHER_FLAG TEST_SOURCE)
    set(original_flags "${CMAKE_REQUIRED_FLAGS}")

    if (MSVC)
        set(required_flags "${MSVC_FLAG}")
    else ()
        set(required_flags "${OTHER_FLAG}")
    endif ()

    set(CMAKE_REQUIRED_FLAGS "${required_flags}")
    include(CheckCXXSourceCompiles)
    check_cxx_source_compiles("${TEST_SOURCE}" ${RESULT_VAR})

    set(CMAKE_REQUIRED_FLAGS "${original_flags}")
    set(${RESULT_VAR} ${${RESULT_VAR}} PARENT_SCOPE)
endfunction()

function(check_all_simd_features)
    # SIMD test definitions
    set(SSE_TEST "
        #include <xmmintrin.h>
        void floats_add(float *dest, float *a, float *b, unsigned size) {
            for(; size >= 4; size -= 4, dest += 4, a += 4, b += 4)
                _mm_storeu_ps(dest, _mm_add_ps(_mm_loadu_ps(a), _mm_loadu_ps(b)));
        }
        int main() { floats_add(0,0,0,0); return 0; }")

    set(SSE2_TEST "
        #include <emmintrin.h>
        void doubles_add(double *dest, double *a, double *b, unsigned size) {
            for(; size >= 2; size -= 2, dest += 2, a += 2, b += 2)
                _mm_store_pd(dest, _mm_add_pd(_mm_loadu_pd(a), _mm_loadu_pd(b)));
        }
        int main() { doubles_add(0,0,0,0); return 0; }")

    set(AVX_TEST "
        #include <immintrin.h>
        void floats_add(float *dest, float *a, float *b, unsigned size) {
            for(; size >= 8; size -= 8, dest += 8, a += 8, b += 8)
                _mm256_storeu_ps(dest, _mm256_add_ps(_mm256_loadu_ps(a), _mm256_loadu_ps(b)));
        }
        int main() { floats_add(0,0,0,0); return 0; }")

    set(AVX2_TEST "
        #include <immintrin.h>
        void ints_add(int *dest, int *a, int *b, unsigned size) {
            for(; size >= 8; size -= 8, dest += 8, a += 8, b += 8)
                _mm256_storeu_si256((__m256i*)dest,
                    _mm256_add_epi32(_mm256_loadu_si256((__m256i*)a),
                                    _mm256_loadu_si256((__m256i*)b)));
        }
        int main() { ints_add(0,0,0,0); return 0; }")

    set(AVX512_TEST "
        #include <immintrin.h>
        void floats_add(float *dest, float *a, float *b, unsigned size) {
            for(; size >= 16; size -= 16, dest += 16, a += 16, b += 16)
                _mm512_storeu_ps(dest, _mm512_add_ps(_mm512_loadu_ps(a), _mm512_loadu_ps(b)));
        }
        int main() { floats_add(0,0,0,0); return 0; }")

    # Perform SIMD checks and propagate results to parent scope
    check_simd_support(HAS_SSE "SSE" "/arch:SSE" "-msse" "${SSE_TEST}")
    set(HAS_SSE ${HAS_SSE} PARENT_SCOPE)

    check_simd_support(HAS_SSE2 "SSE2" "/arch:SSE2" "-msse2" "${SSE2_TEST}")
    set(HAS_SSE2 ${HAS_SSE2} PARENT_SCOPE)

    check_simd_support(HAS_AVX "AVX" "/arch:AVX" "-mavx" "${AVX_TEST}")
    set(HAS_AVX ${HAS_AVX} PARENT_SCOPE)

    check_simd_support(HAS_AVX2 "AVX2" "/arch:AVX2" "-mavx2" "${AVX2_TEST}")
    set(HAS_AVX2 ${HAS_AVX2} PARENT_SCOPE)

    check_simd_support(HAS_AVX512F "AVX512F" "/arch:AVX512" "-mavx512f" "${AVX512_TEST}")
    set(HAS_AVX512F ${HAS_AVX512F} PARENT_SCOPE)
endfunction()

function(print_simd_support)
    # Determine highest supported instruction set
    set(SIMD_INSTRUCTION_TYPE "None")

    if (HAS_SSE)
        message(STATUS "SSE supported")
        #set(SIMD_INSTRUCTION_TYPE "SSE")
    else ()
        message(WARNING "SSE not supported")
    endif ()

    if (HAS_SSE2)
        message(STATUS "SSE2 supported")
        #set(SIMD_INSTRUCTION_TYPE "SSE2")
    else ()
        message(WARNING "SSE2 not supported")
    endif ()

    if (HAS_AVX)
        message(STATUS "AVX supported")
        #set(SIMD_INSTRUCTION_TYPE "AVX")
    else ()
        message(WARNING "AVX not supported")
    endif ()

    if (HAS_AVX2)
        message(STATUS "AVX2 supported")
        set(SIMD_INSTRUCTION_TYPE "AVX2")
    else ()
        message(WARNING "AVX2 not supported")
    endif ()

    if (HAS_AVX512F)
        message(STATUS "AVX512F supported")
        #set(SIMD_INSTRUCTION_TYPE "AVX512F")
    else ()
        message(WARNING "AVX512F not supported")
    endif ()
    set(SIMD_INSTRUCTION_TYPE ${SIMD_INSTRUCTION_TYPE} PARENT_SCOPE)
endfunction()
function(set_simd_instructions target_name)

    if (SIMD_INSTRUCTION_TYPE STREQUAL "AVX2")
        target_compile_options(${target_name} PRIVATE
                $<$<CXX_COMPILER_ID:MSVC>:/arch:AVX2>
                $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-mavx2>
        )
    elseif (SIMD_INSTRUCTION_TYPE STREQUAL "AVX")
        target_compile_options(${target_name} PRIVATE
                $<$<CXX_COMPILER_ID:MSVC>:/arch:AVX>
                $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-mavx>
        )
    elseif (SIMD_INSTRUCTION_TYPE STREQUAL "AVX512F")
        target_compile_options(${target_name} PRIVATE
                $<$<CXX_COMPILER_ID:MSVC>:/arch:AVX512>
                $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-mavx512f>
        )
    elseif (SIMD_INSTRUCTION_TYPE STREQUAL "SSE2")
        target_compile_options(${target_name} PRIVATE
                $<$<CXX_COMPILER_ID:MSVC>:/arch:SSE2>
                $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-msse2>
        )
    elseif (SIMD_INSTRUCTION_TYPE STREQUAL "SSE")
        target_compile_options(${target_name} PRIVATE
                $<$<CXX_COMPILER_ID:MSVC>:/arch:SSE>
                $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-msse>
        )
    endif ()

    # Compiler compatibility check
    if (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
        message(STATUS "Set SIMD instructions to ${SIMD_INSTRUCTION_TYPE} for target '${target_name}' with '${CMAKE_CXX_COMPILER_ID}' compiler.")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(STATUS "Set SIMD instructions to ${SIMD_INSTRUCTION_TYPE} for target '${target_name}' with '${CMAKE_CXX_COMPILER_ID}' compiler.")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND MSVC_VERSION GREATER 1900)
        message(STATUS "Set SIMD instructions to ${SIMD_INSTRUCTION_TYPE} for target '${target_name}' with '${CMAKE_CXX_COMPILER_ID}' compiler.")
    else ()
        message(STATUS "Cannot set SIMD instructions to ${SIMD_INSTRUCTION_TYPE} for target '${target_name}' with '${CMAKE_CXX_COMPILER_ID}' compiler.")
    endif ()

endfunction()