# blam_inflater
A tool to decompress various Halo files.

This tool was created to inflate (or decompress) some files that Halo 3 uploads to Bungie's web services.
However, it should suite any file compressed via the `runtime_data_compress` function in Halo 3 and Reach. I have not checked older games.

To use the tool, simply provide your compressed file path as an arguemnt, and it will be inflated in place.

This tool uses zlib 1.1.4 (the same version Halo 3 shipped with)
