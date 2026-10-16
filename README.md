# rfactor2-udp-broadcaster

- Supply rFactor 2 SDK headers under their permitted terms. Set `RFACTOR2_SDK_DIR` to their directory.
- Supply compatible Protobuf 3.5.1 x64 libraries. Set `PROTOBUF_LIB_DIR` to the directory that contains `libprotobuf.lib` and `libprotobufd.lib`.
- Put a compatible `protoc` executable on `PATH`. Run `rFactor2UdpBroadcaster/generateproto.bat` if you need to regenerate the protocol source.
- Build with the Visual Studio v120 toolset. The C# reader uses its declared NuGet dependency.

The SDK headers and binary dependencies are excluded from this source copy.
The bundled Protobuf source retains its existing license notices.

## Community use

RaceHub source contributions are available under the scope in [LICENSING.md](LICENSING.md).
Local features do not require a RaceHub subscription.
Third-party games, SDKs, and optional hosting keep their own terms and costs.
See [CONTRIBUTING.md](CONTRIBUTING.md) and [SECURITY.md](SECURITY.md).
No hosted service or maintenance schedule is included.
