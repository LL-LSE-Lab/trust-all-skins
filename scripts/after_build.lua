import("core.base.json")

function pack_mod(target, opt)
    local bindir = target:targetdir()
    local moddir = path.join(bindir, target:name())
    
    os.mkdir(moddir)
    
    os.cp(target:targetfile(), moddir)
    
    -- Generate manifest.json
    local manifest = {
        name = opt.modName,
        entry = opt.modFile,
        version = opt.modVersion,
        type = "native"
    }
    
    local manifestPath = path.join(moddir, "manifest.json")
    json.savefile(manifestPath, manifest)
    
    print("Mod packed to: " .. moddir)
end
