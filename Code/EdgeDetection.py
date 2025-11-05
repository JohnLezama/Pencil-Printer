import cv2
import numpy as np

image = cv2.imread('IronManMask.jpg')
if int(image.shape[1])>=int(image.shape[0]):
    cv2.imwrite('ImageOut.png', cv2.resize(image, (int(image.shape[0]*400/image.shape[1]), 400)))
else:
    cv2.imwrite('ImageOut.png', cv2.resize(image, (400, int(image.shape[1]*400/image.shape[0]))))
image = cv2.imread('ImageOut.png')
gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
edges = cv2.Canny(gray_image, 100, 200)
cv2.imwrite('output_edges.png', edges)

binary = cv2.imread("output_edges.png", cv2.IMREAD_GRAYSCALE)
ys, xs = np.where(binary == 255)

# Combine into list of (x, y) tuples
white_pixels = [(int(x), int(y)) for x, y in zip(xs, ys)]
YIndices = set(ys)
with open("output.txt", "w") as f:
    for yIndex in YIndices:
        f.write(str(yIndex)+": (")
        for item in white_pixels:
            if item[1] == yIndex:
                f.write(str(item[0])+", ")
        f.write(")\n")