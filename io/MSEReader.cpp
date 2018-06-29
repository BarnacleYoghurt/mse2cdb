#include <stdexcept>
#include "MSEReader.hpp"

namespace io{

MSEReader::MSEReader(std::string msePath) {
    int zipErrCode = 0;
    mseFile = zip_open(msePath.c_str(),ZIP_RDONLY,&zipErrCode);
    if (zipErrCode > 0){
        throw std::runtime_error("Could not open .mse file for reading (" + getErrorMessage(zipErrCode) + ")");
    }
}

MSEReader::~MSEReader() {
    if (mseFile != nullptr) {
        zip_discard(mseFile);
    }
}

std::string MSEReader::getSetData() {
    zip_file *set = zip_fopen(mseFile, "set", ZIP_RDONLY);
    if (set == nullptr) {
        int errCode;
        zip_error_get(mseFile,&errCode,nullptr);
        throw std::runtime_error("Could not open set file inside .mse for reading (" + getErrorMessage(errCode) + ")");
    }
    struct zip_stat setStats;
    if (zip_stat(mseFile, "set", ZIP_STAT_SIZE, &setStats) == -1){
        int errCode;
        zip_error_get(mseFile,&errCode,nullptr);
        throw std::runtime_error("Could not determine size of set file (" + getErrorMessage(errCode) + ")");
    }
    auto *buf = (char *)malloc(setStats.size);
    if (buf == nullptr){
        throw std::runtime_error("Could not allocate space for set data.");
    }
    if (zip_fread(set, buf, setStats.size) == -1){
        throw std::runtime_error("Could not read from set file.");
    }
    std::string setData = buf;
    delete buf;
    return setData;
}

std::string MSEReader::getErrorMessage(int errorCode) {
    auto *zipError = (zip_error*)malloc(sizeof(zip_error));
    zip_error_init_with_code(zipError, errorCode);
    std::string errorMessage = zip_error_strerror(zipError);
    zip_error_fini(zipError);
    return errorMessage;
}

}