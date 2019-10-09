// The native SFS page size
//
#define XrdSfsPageSize 4096

//-----------------------------------------------------------------------------
//! Read file pages into a buffer and return corresponding checksums.
//!
//! @param  offset  - The offset where the read is to start. It must be
//!                   page aligned.
//! @param  buffer  - pointer to buffer where the bytes are to be placed.
//! @param  rdlen   - The number of bytes to read. The amount must be an
//!                   integral number of XrdSfsPageSize bytes.
//! @param  csvec   - A vector of [rdlen/XrdSfsPageSize] entries which will be
//!                   filled with the corresponding CRC32 checksum for each
//!                   page. A nil pointer does not return the checksums.
//! @param  verify  - When true, the checksum is verified for each page; an
//!                   error is returned if any checksum is incorrect.
//!
//! @return >= 0      The number of bytes that placed in buffer.
//! @return SFS_ERROR File could not be read, error holds the reason.
//-----------------------------------------------------------------------------

virtual XrdSfsXferSize pgRead(XrdSfsFileOffset   offset,
                              char              *buffer,
                              XrdSfsXferSize     rdlen,
                              uint32_t          *csvec,
                              bool               verify=true);

//-----------------------------------------------------------------------------
//! Read file pages and checksums using asynchronous I/O.
//!
//! @param  aioparm - Pointer to async I/O object controlling the I/O.
//! @param  verify  - When true, the checksum is verified for each page; an
//!                   error is returned if any checksum is incorrect.
//!
//! @return SFS_OK    Request accepted and will be scheduled.
//! @return SFS_ERROR File could not be read, error holds the reason.
//-----------------------------------------------------------------------------

virtual int            pgRead(XrdSfsAio *aioparm, bool verify=true);

//-----------------------------------------------------------------------------
//! Write file pages into a file with corresponding checksums.
//!
//! @param  offset  - The offset where the write is to start. It must be
//!                   page aligned.
//! @param  buffer  - pointer to buffer containing the bytes to write.
//! @param  wrlen   - The number of bytes to write. If amount is not an
//!                   integral number of XrdSfsPageSize bytes, then this must
//!                   be the last write to the file at or above the offset.
//! @param  csvec   - A vector of [CEILING(wrlen/XrdSfsPageSize)] entries which
//!                   contain the corresponding CRC32 checksum for each page.
//!                   A nil pointer causes the checksums to be computed.
//! @param  verify  - When true, the checksum in csvec is verified for each
//!                   page; and error is returned if any checksum is incorrect.
//!
//! @return >= 0      The number of bytes written.
//! @return SFS_ERROR File could not be read, error holds the reason.
//-----------------------------------------------------------------------------

virtual XrdSfsXferSize pgWrite(XrdSfsFileOffset   offset,
                               char              *buffer,
                               XrdSfsXferSize     wrlen,
                               uint32_t          *csvec,
                               bool               verify=true);

//-----------------------------------------------------------------------------
//! Write file pages and checksums using asynchronous I/O.
//!
//! @param  aioparm - Pointer to async I/O object controlling the I/O.
//! @param  verify  - When true, the checksum in csvec is verified for each
//!                   page; and error is returned if any checksum is incorrect.
//!
//! @return SFS_OK    Request accepted and will be scheduled.
//! @return SFS_ERROR File could not be read, error holds the reason.
//-----------------------------------------------------------------------------

virtual int            pgWrite(XrdSfsAio *aioparm, bool verify=true);

//! Read file bytes using asynchronous I/O.
virtual int            read(XrdSfsAio *aioparm) = 0;
                             int               rdvCnt);
//! Write file bytes using asynchronous I/O.
                              int               wdvCnt);
//-----------------------------------------------------------------------------
//! Notify filesystem that a client has connected.
//!
//! @param  client - Client's identify (see common description).
//-----------------------------------------------------------------------------

virtual void           Connect(const XrdSecEntity     *client = 0)
{
  (void)client;
}

//-----------------------------------------------------------------------------
//! Return directory/file existence information (short stat).
//!
//! @param  path   - Pointer to the path of the file/directory in question.
//! @param  eFlag  - Where the results are to be returned.
//! @param  eInfo  - The object where error info is to be returned.
//! @param  client - Client's identify (see common description).
//! @param  opaque - Path's CGI information (see common description).
//!
//! @return One of SFS_OK, SFS_ERROR, SFS_REDIRECT, SFS_STALL, or SFS_STARTED
//!         When SFS_OK is returned, eFlag must be properly set, as follows:
//!         XrdSfsFileExistNo            - path does not exist
//!         XrdSfsFileExistIsFile        - path refers to an  online file
//!         XrdSfsFileExistIsDirectory   - path refers to an  online directory
//!         XrdSfsFileExistIsOffline     - path refers to an offline file
//!         XrdSfsFileExistIsOther       - path is neither a file nor directory
//-----------------------------------------------------------------------------

virtual int            exists(const char                *path,
                                    XrdSfsFileExistence &eFlag,
                                    XrdOucErrInfo       &eInfo,
                              const XrdSecEntity        *client = 0,
                              const char                *opaque = 0) = 0;

//! @return The bit-wise feature set (i.e. supported or configured).
//!         See include file XrdSfsFlags.hh for actual bit values.
//! Perform a third party file transfer or cancel one.
//! @param  gpAct  - What to do as one of the enums listed below.
//! @param  gpReq  - reference tothe object describing the request. This object
//!                  is also used communicate the request status.
//! @param  eInfo  - The object where error info or results are to be returned.
//! @return SFS_OK   Request accepted (same as SFS_STARTED). Otherwise, one of
//!                  SFS_ERROR, SFS_REDIRECT, or SFS_STALL.
enum gpfFunc {gpfCancel=0, //!< Cancel this request
              gpfGet,      //!< Perform a file retrieval
              gpfPut       //!< Perform a file push
             };

virtual int            gpFile(      gpfFunc          &gpAct,
                                    XrdSfsGPFile     &gpReq,
                                    XrdOucErrInfo    &eInfo,
                              const XrdSecEntity     *client = 0)
                             {(void)gpAct, (void)gpReq; (void)client;
                              eInfo.setErrInfo(ENOTSUP, "Not supported.");
                              return SFS_ERROR;
                             }