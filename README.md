# Memelord

A simple search tool for memes.

Memelord extracts text from the images in a folder and indexes them. It can then search the folder for images by keyword.

## Usage

### Indexing

Indexing will add a folder `.meme_index` to the specified directory which contains a list of tags for each image file found in that folder.  

```$bash
memelord index <meme folder>
```

### Searching

The search function will print a list of files for which an exact match for the search term was found.

```$bash
memelord search <meme folder> <search term>
```