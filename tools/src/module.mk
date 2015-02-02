#Source files to compile
FILES := AnaUtils
FILES += DataBaseManager
FILES += Dataset
FILES += Debug
FILES += HistoManager
FILES += HistoUtils
FILES += Sample
FILES += StatUtils

# Header files to use for dictionary generation
DICTFILES := $(FILES) LinkDef
