__kernel void blur(__global uchar* input, __global uchar* output, int width, int height) {
    int idx = get_global_id(0);
    int x = idx % width;
    int y = idx / width;

    if (x <= 0 || y <= 0 || x >= width-1 || y >= height-1) {
        output[idx] = input[idx];
        return;
    }

    int sum = 0;
    for (int oy = -1; oy <= 1; oy++) {
        for (int ox = -1; ox <= 1; ox++) {
            sum += input[(y + oy) * width + (x + ox)];
        }
    }
    output[idx] = (uchar)(sum / 9);
}
