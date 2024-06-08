@echo off
setlocal

rem 현재 디렉토리를 PROTO_DIR과 OUTPUT_DIR로 설정
set PROTO_DIR=%cd%
set OUTPUT_DIR=%cd%

rem PROTO_DIR 내의 모든 .proto 파일을 순회하면서 C# 파일로 변환
for %%f in (%PROTO_DIR%\*.proto) do (
    echo 변환 중: %%f
    protoc --proto_path=%PROTO_DIR% --csharp_out=%OUTPUT_DIR% %%f
    if %errorlevel% neq 0 (
        echo 오류 발생: %%f 변환 실패
        exit /b %errorlevel%
    )
)

endlocal
pause