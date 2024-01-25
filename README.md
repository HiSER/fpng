### Original repository
```
https://github.com/richgel999/fpng
```

### MSVC WinCE Example
```c++
void drawPNG(const char* filename)
{
    auto png = new PNGDecode(filename);
    png->draw(hdc, 100, 100);
    delete png;
}
```
