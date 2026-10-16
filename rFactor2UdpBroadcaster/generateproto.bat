cd /d "%~dp0"
protoc --cpp_out=. telemetry.proto --csharp_out=../rFactor2ConsoleReader
