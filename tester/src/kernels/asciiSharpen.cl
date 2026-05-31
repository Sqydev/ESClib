__kernel void sharpen(__global uchar* input, __global uchar* output, int width, int height) {
    int idx = get_global_id(0);
    int x = idx % width;
    int y = idx / width;

    if (x <= 0 || y <= 0 || x >= width-1 || y >= height-1) {
        output[idx] = input[idx];
        return;
    }

    int v = 5 * input[y*width + x]
          - input[(y-1)*width + x]
          - input[(y+1)*width + x]
          - input[y*width + (x-1)]
          - input[y*width + (x+1)];

    if (v < 0)   v = 0;
    if (v > 255) v = 255;
    output[idx] = (uchar)v;
}
