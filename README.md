# RAWLS reader

## Description

Can read and flip horizontally a `.rawls` image.
Can read and convert `.rawls` generated syntehsis image format into `.ppm` or `.png`.

`.rawls` store all pixels values as *float* and keeps also information about generated image (renderer used).

`.rawls` contains 3 blocks and specific lines information within each block:
- IHDR: 
    - **First line:** next information line size into byte
    - **Second line:** `{width}` `{height}` `{nbChannels}`
- COMMENTS
    - **All lines:** information from the renderer as comment
- DATA
    - **First line:** data block size
    - **Next lines:** all pixels values as byte code at each line

## How it works ?

```
mkdir build
cd build && cmake ..
make 
```

### Examples:

Convert the `.rawls` image format into `.ppm` 
```
./main/rawls_convert --image ../path/to/image.rawls --outfile image.ppm
```


Merge `.rawls` images samples
```
./main/rawls_merge_mean --folder ../path/to/images --samples 100 --random 0 --outfile image.png
```