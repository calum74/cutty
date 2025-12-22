#include <cutty/print.hpp>
#include <cutty/tags.hpp>
#include <cutty/units.hpp>

namespace cy = cutty;
using namespace cutty::literals;

using ChunkSize = cy::tagged<size_t, cy::byte>;
using CompressionLevel = cy::tagged<int, struct comp_level_t>;
using Blocking = cy::tagged<bool, struct blocking_t>;
using Hostname = cy::tagged<std::string, struct hostname_t>;
using Uri = cy::tagged<std::string, struct uri_t>;
using Port = cy::tagged<int, struct port_t>;
using Ssl = cy::tagged<bool, struct force_ssl_t>;

void Sleep(std::size_t t)
{
    cy::print("Sleeping for", t, "ms");
}

void wait(cy::tagged<std::size_t, cy::millisecond> timeout, Blocking block)
{
    if (*block)
    {
        Sleep(*timeout);
    }
}

void send(Hostname hostname, Port port, Uri uri, ChunkSize chunksize, Ssl force_ssl, CompressionLevel compression);

int main()
{
    Ssl using_ssl{true};
    wait(5_second, Blocking{true});
}