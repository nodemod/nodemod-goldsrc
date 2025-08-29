{
    global:
        # Essential metamod plugin symbols
        Meta_*;
        GiveFnptrsToDll;
        
        # Export any other symbols your plugin explicitly needs
        # Add more patterns here if needed
        
    local:
        # Hide all other symbols, including C++ runtime symbols
        *;
};