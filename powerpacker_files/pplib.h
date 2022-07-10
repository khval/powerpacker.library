/* pplib 1.2: a simple PowerPacker decompression and decryption library
 * placed in the Public Domain on ??-???-2010 by Stuart Caie.
 */
#ifndef PPLIB_H
#define PPLIB_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_PPLOADDATA

#if 0
#define PPERR_OK           (0) /* no error                  */
#define PPERR_ARGS         (1) /* bad arguments to function */
#define PPERR_OPEN         (2) /* error opening file        */
#define PPERR_READ         (3) /* error reading from file   */
#define PPERR_SEEK         (4) /* error seeking in file     */
#define PPERR_NOMEMORY     (5) /* out of memory             */
#define PPERR_DATAFORMAT   (6) /* error in data format      */
#define PPERR_PASSWORD     (7) /* bad or missing password   */
#define PPERR_DECRUNCH     (8) /* error decrunching data    */
#else
#define PPERR_OK           (0) /* no error                  */
#define PPERR_ARGS         (PP_UNKNOWNPP) /* bad arguments to function */
#define PPERR_OPEN         (PP_OPENERR) /* error opening file        */
#define PPERR_READ         (PP_READERR) /* error reading from file   */
#define PPERR_SEEK         (PP_UNKNOWNPP) /* error seeking in file     */
#define PPERR_NOMEMORY     (PP_NOMEMORY) /* out of memory             */
#define PPERR_DATAFORMAT   (PP_UNKNOWNPP) /* error in data format      */
#define PPERR_PASSWORD     (PP_PASSERR) /* bad or missing password   */
#define PPERR_DECRUNCH     (PP_UNKNOWNPP) /* error decrunching data    */
#endif

/**
 * Loads PowerPacked data from a file.
 *
 * When finished with the decrunched data, free() the buffer.
 *
 * @param filename the file containing PowerPacked data
 * @param buffer   a pointer to where ppLoadData() will store a pointer to
 *                 the decrunched data buffer
 * @param buflen   a pointer to where ppLoadData() will store the length of
 *                 the decrunched data.
 * @param password a null-terminated string, or NULL if no password is given
 * @return 0 if successful, or an error code
 */
// extern int ppLoadData(char *filename, unsigned char **buffer,  unsigned int *buflen, unsigned char *password);


ULONG ppLoadData(char * filename,  ULONG col,  ULONG memtype, UBYTE ** bufferptr, ULONG * lenptr, BOOL (*password_func)(UBYTE *, ULONG)) ;

#endif


/**
 * Decrunches PowerPacked data.
 *
 * PowerPacker data files have the following format:
 * <pre>
 *   1 longword identifier           'PP20', 'PX20' or 'PPLS'
 *  [1 longword length (if 'PPLS')   0xLLLLLLLL]
 *  [1 word checksum (if 'PX20')     0xSSSS]
 *   1 longword efficiency           0xEEEEEEEE
 *   X longwords crunched data       0xCCCCCCCC, 0xCCCCCCCC, ...
 *   1 longword decrunch info        'decrlen' << 8 | '8 bits other info'
 * </pre>
 *
 * A longword is 4 bytes. A word is 2 bytes. The length of the decrunched
 * data is 'decrlen', the upper 24 bits of the decrunch info trailer. All
 * values are big-endian.
 *
 * @param eff         a pointer to the 4-byte efficiency header
 * @param src         a pointer to the start of the crunched data
 * @param dest        a pointer to the start of the decrunched data buffer
 * @param src_len     length of the source data, in bytes, not including the
 *                    efficiency header, nor the decrunch info trailer
 * @param dest_len    length of the decrunched data buffer
 * @return 0 for failure, 1 for success
 */
extern int ppDecrunchBuffer(const unsigned char *eff,
			    const unsigned char *src, unsigned char *dest,
			    unsigned int src_len, unsigned int dest_len);

/**
 * Does the same as ppDecrunchBuffer(), only it expects 'master mode' data.
 * This is a secret mode used in PowerPacker 2.0 - 3.0 packed executables,
 * where the bit used to identify literals is swapped from 0 to 1
 */
extern int ppDecrunchBuffer_m(const unsigned char *eff,
			      const unsigned char *src, unsigned char *dest,
			      unsigned int src_len, unsigned int dest_len);

/**
 * Calculates the 16-bit checksum of a password.
 *
 * This checksum is stored in the header of an encrypted PowerPacker file.
 * PowerPacker passwords should be no longer than 16 characters.
 *
 * @param phrase a null terminated string
 * @return the 16-bit checksum of the password
 */
extern unsigned int ppCalcChecksum(const unsigned char *password);

/**
 * Calculates a 32-bit decryption key from a password.
 *
 * The key is used with ppDecrypt() to decrypt a file. PowerPacker
 * passwords should be no longer than 16 characters.
 *
 * @param phrase a null terminated string
 * @return a 32-bit decryption key
 */
extern unsigned int ppCalcPasskey(const unsigned char *password);

/**
 * Decrypts encrypted PowerPacker data.
 *
 * PowerPacker encrypts data by XORing it with a 32-bit key.
 *
 * @param data a buffer with encrypted PowerPacker data
 * @param the length of the buffer in bytes. This will be rounded up to
 *        the nearest 4 bytes
 * @param key the key to perform the decryption with
 */
extern void ppDecrypt(unsigned char *data, unsigned int len, unsigned int key);

#ifdef __cplusplus
};
#endif
#endif /* PPLIB_H */
