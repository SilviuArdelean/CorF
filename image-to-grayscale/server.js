const express = require('express');
const fileUpload = require('express-fileupload');
const path = require('path');
const fs = require('fs');

const app = express();
const port = 3000;
const addonPath = path.join(__dirname, './cpp_addon/build/Release/image_grayscale.node');

// Middleware
app.use(express.static(path.join(__dirname, 'public')));
app.use(fileUpload());

// Serve static files from the 'uploads' directory
app.use('/uploads', express.static('uploads'));

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// Route for uploading and processing images
app.post('/upload', (req, res) => {
    if (!req.files || !req.files.image) {
        return res.status(400).send('No image uploaded.');
    }

    const imageFile = req.files.image;
    const imagePath = path.join(__dirname, 'uploads', imageFile.name);
    if (fs.existsSync(addonPath)) {
        const { processImage } = require(addonPath);

        // prcess the uploaded image
        imageFile.mv(imagePath, (err) => {
            if (err) {
                return res.status(500).send(err);
            }

            // Process the image using the C++ add-in
            processImage(imagePath, (err, processedImagePath) => {
                if (err) {
                    console.error("Error processing image:", err);
                    return res.status(500).send(err);
                }

                if (!fs.existsSync(processedImagePath)) {
                    console.log("The new generated path is invalid: ", processedImagePath);
                    return;
                }
                // Send the relative URL to the processed image
                const processedImageUrl = `/uploads/${path.basename(processedImagePath)}`;
                res.send({ url: processedImageUrl });
            });
        });
    } else {
        console.error(`The file ${addonPath} does not exist.`);
    }
});

// Start the server
app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});

