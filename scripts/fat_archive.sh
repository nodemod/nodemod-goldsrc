#!/bin/bash

# Script to process all `.a` files recursively in a directory and convert thin archives to fat archives.
# Each converted archive outputs as `_fat.a` in the same directory as the original.
# Usage: ./convert_to_fat_archives.sh /path/to/search/directory

SEARCH_DIR=$1

# Temporary directory to store extracted paths
TEMP_DIR=$(mktemp -d)

# Validate input
if [[ -z "$SEARCH_DIR" ]]; then
    echo "Usage: $0 /path/to/search/directory"
    exit 1
fi

if [[ ! -d "$SEARCH_DIR" ]]; then
    echo "Error: Directory $SEARCH_DIR does not exist."
    exit 1
fi

# Check if `ar` is installed
if ! command -v ar &> /dev/null; then
    echo "Error: 'ar' command not found. Please install it first."
    exit 1
fi

echo "Starting conversion of thin archives in directory: $SEARCH_DIR"
echo "Temporary directory: $TEMP_DIR"

# Find all `.a` files in the search directory
find "$SEARCH_DIR" -type f -name "*.a" | while read -r THIN_ARCHIVE; do

    # Check if the archive exists
    if [[ ! -f "$THIN_ARCHIVE" ]]; then
        echo "Error: Archive $THIN_ARCHIVE does not exist. Skipping."
        continue
    fi

    # Test if the archive is thin
    ARCHIVE_TYPE=$(file "$THIN_ARCHIVE")
    echo "Processing archive: $THIN_ARCHIVE"
    echo "Detected archive type: $ARCHIVE_TYPE"

    if [[ "$ARCHIVE_TYPE" == *thin* ]]; then
        echo " -> Detected THIN archive. Converting to FAT archive..."

        # Extract object file references from the archive
        ar -t "$THIN_ARCHIVE" > "$TEMP_DIR/file_list.txt"

        # Ensure the file list was created
        if [[ ! -s "$TEMP_DIR/file_list.txt" ]]; then
            echo "Error: Failed to extract file references from $THIN_ARCHIVE. Skipping."
            continue
        fi

        # Copy all object files into the temporary directory
        OBJ_FILE_LIST=$(cat "${TEMP_DIR}/file_list.txt")
        echo "Verifying and collecting object files from $THIN_ARCHIVE..."
        for OBJ_FILE in $OBJ_FILE_LIST; do
            if [[ ! -f "$OBJ_FILE" ]]; then
                echo "Error: Missing object file $OBJ_FILE (referenced by $THIN_ARCHIVE). Skipping."
                continue 2
            fi
            cp --parents "$OBJ_FILE" "$TEMP_DIR"
        done

        # Generate the output filename for the fat archive
        FAT_ARCHIVE="${THIN_ARCHIVE%.a}_fat.a"

        # Repack the object files into a new fat archive
        echo " -> Repacking files into a fat archive: $FAT_ARCHIVE"
        cd "$TEMP_DIR" || exit
        ar -crs "$FAT_ARCHIVE" $(cat "$TEMP_DIR/file_list.txt") || {
            echo "Error: Failed to create FAT archive $FAT_ARCHIVE. Skipping."
            continue
        }
        cd - > /dev/null

        echo " -> Successfully converted $THIN_ARCHIVE to $FAT_ARCHIVE."
    else
        echo " -> Skipping: Archive $THIN_ARCHIVE is already a FAT archive."
    fi

done

# Cleanup temporary directory
echo "Cleaning up temporary directory..."
rm -rf "$TEMP_DIR"

echo "Conversion process completed. All thin archives have been processed."
