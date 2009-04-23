/*=========================================================================

  Program: GDCM (Grassroots DICOM). A DICOM library
  Module:  $URL$

  Copyright (c) 2006-2009 Mathieu Malaterre
  All rights reserved.
  See Copyright.txt or http://gdcm.sourceforge.net/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef __gdcmRSA_h
#define __gdcmRSA_h

#include "gdcmTypes.h"

namespace gdcm
{
/**
 * \brief Class for RSA encryption
 *
http://polarssl.org//?page=forum_view_topic&topic_id=12
 */
//-----------------------------------------------------------------------------
class RSAInternals;
class GDCM_EXPORT RSA
{
public :
  RSA();
  ~RSA();

  typedef enum {
/* #define RSA_*/ PUBLIC    = 0,
/* #define RSA_*/ PRIVATE   = 1
  } RSAModeType;

  // 0 is a reserved value do not use
  typedef enum {
/* #define X509_*/ OUTPUT_DER            = 1,
/* #define X509_*/ OUTPUT_PEM            = 2
  } X509FormatType;

  /// Return the length of the key:
  unsigned int GetLenkey() const;

/**
 * \brief          Check a public RSA key
 *
 * \param ctx      RSA context to be checked
 *
 * \return         0 if successful, or an POLARSSL_ERR_RSA_XXX error code
 */
int CheckPubkey( ) const;

/**
 * \brief          Check a private RSA key
 *
 * \param ctx      RSA context to be checked
 *
 * \return         0 if successful, or an POLARSSL_ERR_RSA_XXX error code
 */
int CheckPrivkey() const;

/**
 * \brief          Add the message padding, then do an RSA operation
 *
 * \param ctx      RSA context
 * \param mode     RSA_PUBLIC or RSA_PRIVATE
 * \param ilen     contains the plaintext length
 * \param input    buffer holding the data to be encrypted
 * \param output   buffer that will hold the ciphertext
 *
 * \return         0 if successful, or an XYSSL_ERR_RSA_XXX error code
 *
 * \note           The output buffer must be as large as the size
 *                 of ctx->N (eg. 128 bytes if RSA-1024 is used).
 */
int Pkcs1Encrypt( 
                       int mode, unsigned int ilen,
                       const char *input,
                       char *output ) const;

/**
 * \brief          Do an RSA operation, then remove the message padding
 *
 * \param ctx      RSA context
 * \param mode     RSA_PUBLIC or RSA_PRIVATE
 * \param input    buffer holding the encrypted data
 * \param output   buffer that will hold the plaintext
 * \param olen     will contain the plaintext length
 * \param output_max_len	maximum length of the output buffer
 *
 * \return         0 if successful, or an POLARSSL_ERR_RSA_XXX error code
 *
 * \note           The output buffer must be as large as the size
 *                 of ctx->N (eg. 128 bytes if RSA-1024 is used) otherwise
 *                 an error is thrown.
 */
int Pkcs1Decrypt(
                       int mode, unsigned int &olen,
                       const char *input,
                       char *output,
		                   unsigned int output_max_len);

/**
 * \brief          Load and parse a private RSA key
 *
 * \param rsa      RSA context to be initialized
 * \param path     filename to read the private key from
 * \param pwd      password to decrypt the file (can be NULL)
 *
 * \return         0 if successful, or a specific X509 error code
 */
int X509ParseKeyfile( const char *path, const char *password = 0 );

/*
 * \brief          Write a private RSA key into a file
 *
 * \param rsa      points to an RSA key
 * \param path     filename to write the key to
 * \param format   X509_OUTPUT_DER or X509_OUTPUT_PEM
 *
 * \return         0 if successful, or a specific X509 error code
 */
int X509WriteKeyfile( const char *path,
                       int format = OUTPUT_PEM );

/**
 * \brief          Parse a private RSA key
 *
 * \param rsa      RSA context to be initialized
 * \param buf      input buffer
 * \param buflen   size of the buffer
 * \param pwd      password for decryption (optional)
 * \param pwdlen   size of the password
 *
 * \return         0 if successful, or a specific X509 error code
 */
int X509ParseKey(
                   const char *buf, unsigned int buflen,
                   const char *pwd = 0, unsigned int pwdlen = 0 );

protected:
int SelfTest( int verbose = 0 ) const;

private:
  RSAInternals *Internals;
private:
  RSA(const RSA&);  // Not implemented.
  void operator=(const RSA&);  // Not implemented.
};
} // end namespace gdcm
//-----------------------------------------------------------------------------
#endif //__gdcmRSA_h
