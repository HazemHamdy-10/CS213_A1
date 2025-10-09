// Name: Mohamed Khaled Anwar
// Id: 20240493

#include <bits/stdc++.h>
#include "Image_Class.h"

using namespace std;

Image img;
bool loaded = false;
bool modified = false;

// check valid extension
bool validExtension(const string &name) {
    string exts[] = {".jpg", ".png", ".bmp", ".jpeg"};
    for (auto &e: exts) {
        if (name.size() >= e.size() && equal(e.rbegin(), e.rend(), name.rbegin()))
            return true;
    }
    return false;
}

bool askToSave() {
    if (loaded && modified) {
        char c;
        cout << "Do you want to save the current image before proceeding? (y/n): ";
        cin >> c;
        if (c == 'y' || c == 'Y') {
            string name;
            cout << "Enter filename to save (with extension): ";
            cin >> name;
            if (!validExtension(name)) {
                cout << "Invalid extension!\n";
                return false;
            }
            if (img.saveImage(name)) {
                cout << "Image saved as " << name << endl;
                modified = false;
            } else {
                cout << "Failed to save!\n";
                return false;
            }
        }
    }
    return true;
}

void loadImage() {
    if (!askToSave())
        return;
    string name;
    cout << "Enter image name (with extension): ";
    cin >> name;
    if (!validExtension(name)) {
        cout << "Invalid extension!\n";
        return;
    }
    if (img.loadNewImage(name)) {
        cout << "Image loaded successfully!\n";
        loaded = true;
        modified = false;
    } else {
        cout << "Failed to load image!\n";
    }
}

void saveImage() {
    if (!loaded) {
        cout << "No image loaded!\n";
        return;
    }
    string name;
    cout << "Enter filename to save (with extension): ";
    cin >> name;
    if (!validExtension(name)) {
        cout << "Invalid extension!\n";
        return;
    }
    if (img.saveImage(name)) {
        cout << "Saved as " << name << endl;
        modified = false;
    } else {
        cout << "Failed to save!\n";
    }
}

// Filter 2: Black and White
void blackWhite() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }

    for (int i = 0; i < img.width; ++i) {
        for (int j = 0; j < img.height; ++j) {

            unsigned int avg = 0;
            for (int k = 0; k < img.channels; ++k) {
                avg += img(i, j, k);
            }
            avg = avg / img.channels;

            for (int k = 0; k < img.channels; ++k) {
                if (avg < 128)
                    img(i, j, k) = 0;
                else
                    img(i, j, k) = 255;
            }
        }
    }

    cout << "Black & White applied.\n";
    modified = true;
}

// Filter 5: Flip Image
void flip() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }
    char your_choice;
    cout << "Flip (H/V): ";
    cin >> your_choice;

    if (your_choice == 'H' || your_choice == 'h') {
        for (int i = 0; i < img.width / 2; ++i) {
            int i2 = img.width - 1 - i;
            for (int j = 0; j < img.height; ++j) {
                for (int k = 0; k < img.channels; ++k) {
                    int tmp = img(i, j, k);
                    img(i, j, k) = img(i2, j, k);
                    img(i2, j, k) = tmp;
                }
            }
        }
        cout << "Flipped Horizontally.\n";
    } else {
        for (int i = 0; i < img.width; ++i) {
            for (int j = 0; j < img.height / 2; ++j) {
                int j2 = img.height - 1 - j;
                for (int k = 0; k < img.channels; ++k) {
                    int tmp = img(i, j, k);
                    img(i, j, k) = img(i, j2, k);
                    img(i, j2, k) = tmp;
                }
            }
        }
        cout << "Flipped Vertically.\n";
    }
    modified = true;
}

// Filter 8: Crop Images
void crop() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }

    cout << "Original image size: " << img.width << " x " << img.height << endl;

    int x, y, w, h;
    cout << "Enter the starting point (x y) of the upper-left corner:\n";
    cin >> x >> y;
    cout << "Enter the width and height (W H) of the area to crop:\n";
    cin >> w >> h;

    if (x < 0 || y < 0 || w <= 0 || h <= 0 || x + w > img.width || y + h > img.height) {
        cout << "Invalid crop dimensions. They must fit within the original image.\n";
        return;
    }

    Image cropped(w, h);

    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            for (int c = 0; c < img.channels; ++c) {
                cropped(i, j, c) = img(x + i, y + j, c);
            }
        }
    }

        img = cropped;
        loaded = true;
        cout << "Image cropped successfully.\n";
        cout << "New image size: " << w << " x " << h << endl;
        modified = true;

}

// Filter 11: Resizing Images
void resizeImage() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }

    int newWidth, newHeight;
    cout << "Enter the new width and height of the image:\n";
    cin >> newWidth >> newHeight;

    if (newWidth <= 0 || newHeight <= 0) {
        cout << "Invalid dimensions!\n";
        return;
    }

    // Create a new image with the desired size
    Image resizedImage(newWidth, newHeight);

    // Compute scale factors
    float x_ratio = static_cast<float>(img.width) / newWidth;
    float y_ratio = static_cast<float>(img.height) / newHeight;

    // Nearest Neighbor Interpolation
    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            int srcX = static_cast<int>(x * x_ratio);
            int srcY = static_cast<int>(y * y_ratio);

            for (int c = 0; c < img.channels; ++c) {
                resizedImage(x, y, c) = img(srcX, srcY, c);
            }
        }
    }


    img = resizedImage;
    loaded = true;
    cout << "Image resized successfully!\n";
    cout << "Original size: " << img.width << "x" << img.height << endl;
    cout << "New size: " << newWidth << "x" << newHeight << endl;
    modified = true;

}

// Filter 16: looking so purple at night
void wanoPurple() {
    if (!loaded) {
        cout << "Load an image first!\n";
        return;
    }

    if (img.width <= 0 || img.height <= 0 || img.channels <= 0) {
        cout << "Invalid image properties!\n";
        return;
    }

    for (int i = 0; i < img.width; ++i) {
        for (int j = 0; j < img.height; ++j) {
            int red = img(i, j, 0);
            int green = img(i, j, 1);
            int blue = img(i, j, 2);

            int newRed = min(255, red + 60);
            int newGreen = max(0, green - 20);
            int newBlue = min(255, blue + 40);

            img(i, j, 0) = (newRed + red) / 2;
            img(i, j, 1) = newGreen;
            img(i, j, 2) = (newBlue + blue) / 2;
        }
    }

    cout << "Wano purple night filter applied successfully!\n";
    modified = true;

}

int main() {
    cout << "=== Welcome to Image Processing Program ===\n";
    loadImage();  // must load at start

    int choice;
    do {
        cout << "\n==== Menu ====\n";
        cout << "1. Load New Image\n";
        cout << "2. Black & White\n";
        cout << "3. Flip\n";
        cout << "4. Crop\n";
        cout << "5. Resize\n";
        cout << "6. Wano Purple Night\n";
        cout << "7. Save Image\n";
        cout << "8. Exit\n";
        cout << "Choose option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                loadImage();
                break;
            case 2:
                blackWhite();
                break;
            case 3:
                flip();
                break;
            case 4:
                crop();
                break;
            case 5:
                resizeImage();
                break;
            case 6:
                wanoPurple();
                break;
            case 7:
                saveImage();
                break;
            case 8:
                if (!askToSave())
                    break;
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 8);

    return 0;
}
