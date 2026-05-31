__kernel void edge(__global uchar* input, __global uchar* output, int width, int height) {
    int idx = get_global_id(0);
    int x = idx % width;
    int y = idx / width;

    if (x <= 0 || y <= 0 || x >= width-1 || y >= height-1) {
        output[idx] = 0;
        return;
    }

    int gx = -input[(y-1)*width + (x-1)] - 2*input[y*width + (x-1)] - input[(y+1)*width + (x-1)]
             +input[(y-1)*width + (x+1)] + 2*input[y*width + (x+1)] + input[(y+1)*width + (x+1)];

    int gy = -input[(y-1)*width + (x-1)] - 2*input[(y-1)*width + x] - input[(y-1)*width + (x+1)]
             +input[(y+1)*width + (x-1)] + 2*input[(y+1)*width + x] + input[(y+1)*width + (x+1)];

    int mag = abs(gx) + abs(gy);
    if (mag > 255) mag = 255;
    output[idx] = (uchar)mag;
}
