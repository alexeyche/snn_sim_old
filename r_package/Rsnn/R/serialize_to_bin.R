#!/usr/bin/RScript
saveMatrixList <- function(baseName, mtxList) {
    idxName <- paste(baseName, ".idx", sep="")
    idxCon <- file(idxName, 'wb')
    on.exit(close(idxCon))

    dataName <- paste(baseName, ".bin", sep="")
    con <- file(dataName, 'wb')
    on.exit(close(con))

    writeBin(0L, idxCon)

    for (m in mtxList) {
        writeBin(dim(m), con)
        writeBin(typeof(m), con)
        writeBin(c(m), con)
        flush(con)

        offset <- as.integer(seek(con))
#        cat('offset', offset)
        writeBin(offset, idxCon)
    }

    flush(idxCon)
}

substrRight <- function(x, n){
  substr(x, nchar(x)-n+1, nchar(x))
}

loadMatrix <- function(baseName = "data", index) {
    if(substrRight(baseName,4) == ".bin") {
        baseName = substr(baseName, 0, nchar(baseName)-4)
    }
    idxName <- paste(baseName, ".idx", sep="")
    idxCon <- file(idxName, 'rb')
    on.exit(close(idxCon))

    dataName <- paste(baseName, ".bin", sep="")
    con <- file(dataName, 'rb')
    on.exit(close(con))

    seek(idxCon, (index-1)*4)
    offset <- readBin(idxCon, 'integer')

    seek(con, offset)
    d <- readBin(con, 'integer', 2)
    type <- readBin(con, 'character', 1)
    if(type == "double:t") {
        type = "double"
        b = d[1] 
        d[1] = d[2]
        d[2] = b
        m = t(structure(readBin(con, type, prod(d)), dim=d))
    } else {
        m = structure(readBin(con, type, prod(d)), dim=d)
    }

    close(idxCon)
    return(m)
}
