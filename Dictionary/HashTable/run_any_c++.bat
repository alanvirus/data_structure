@echo off
@REM set "PROGRAMS=bm_match_normal_v2 match match_with_rolling_hash match_with_rolling_hash_v2 test_pattern_match_speed match_boyer_moore match_kmp"
set "PROGRAMS=FermatLagrange"
for %%P in (%PROGRAMS%) do (
    g++ %%P.cpp -o %%P
    if %errorlevel% equ 0 (
        %%P
    )
)