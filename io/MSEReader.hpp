#ifndef MSE2CDB_MSEREADER_HPP
#define MSE2CDB_MSEREADER_HPP


#include <string>
#include <zip.h>
namespace io {
    class MSEReader {
    public:
        /**
         * @brief Initialize the reader by opening the .mse file at the provided path.
         * @param msePath The path of the .mse file.
         * @throw runtime_error if the .mse file cannot be opened for reading.
         */
        MSEReader(std::string msePath);

        /**
         * @brief Finalize the reader by discarding the opened file.
         */
        ~MSEReader();

        /**
         * @brief Read the full contents of the set file inside the .mse file.
         * @return The contents of the set file.
         * @trhow runtime_error if an error occurs while opening or reading the set file.
         */
        std::string getSetData();

    private:
        std::string getErrorMessage(int errorCode);

        zip *mseFile = nullptr;
    };
}

#endif //MSE2CDB_MSEREADER_HPP
