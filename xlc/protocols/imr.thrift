namespace cpp dan.imr.thrift
namespace java dan.imr.thrift

struct Project {
  1: string name,
  2: i16 majorVersion,
  3: i16 minorVersion
}

service InMemoryRepo {
    void ping(),
    Project getProject(1: string name),
    oneway void advanceMajorVersion(1: string name),
    oneway void advanceMinorVersion(1: string name)
}
