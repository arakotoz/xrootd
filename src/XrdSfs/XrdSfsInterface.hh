class  XrdOucTList;
//! Read file bytes using asynchrnous I/O.
virtual XrdSfsXferSize read(XrdSfsAio *aioparm) = 0;
                             int               rdvCnt)
                            {XrdSfsXferSize rdsz, totbytes = 0;
                             for (int i = 0; i < rdvCnt; i++)
                                 {rdsz = read(readV[i].offset,
                                              readV[i].data, readV[i].size);
                                  if (rdsz != readV[i].size)
                                     {if (rdsz < 0) return rdsz;
                                      error.setErrInfo(ESPIPE,"read past eof");
                                      return SFS_ERROR;
                                     }
                                  totbytes += rdsz;
                                 }
                             return totbytes;
                            }
//! Write file bytes using asynchrnous I/O.
                              int               wdvCnt)
                             {XrdSfsXferSize wrsz, totbytes = 0;
                              for (int i = 0; i < wdvCnt; i++)
                                  {wrsz = write(writeV[i].offset,
                                                writeV[i].data, writeV[i].size);
                                   if (wrsz != writeV[i].size)
                                      {if (wrsz < 0) return wrsz;
                                      error.setErrInfo(ESPIPE,"write past eof");
                                      return SFS_ERROR;
                                     }
                                  totbytes += wrsz;
                                 }
                             return totbytes;
                            }
//!                  for compatability purposes that results in the directory
//!                  object to be larger than needed because it would also
//!                  supply an errinfo object. You can override this method
//!                  in order to return a much smaller object w/o errinfo.
                               {XrdSfsDirectory *dP = newDir();
                                if (dP) dP->error = eInfo;
                                return dP;
                               }
//!                  compatability purposes results in the new file object
//!                  to be larger than needed because it would also supply
//!                  an errinfo object. You can override this method in order
//!                  to return a much smaller object by omitting the errinfo.
                                {XrdSfsFile *fP = newFile();
                                 if (fP) fP->error = eInfo;
                                 return fP;
                                }
//! Return directory/file existence information (short stat).
//! @param  path   - Pointer to the path of the file/directory in question.
//! @param  eFlag  - Where the results are to be returned.
//! @param  eInfo  - The object where error info is to be returned.
//! @param  opaque - Path's CGI information (see common description).
//! @return One of SFS_OK, SFS_ERROR, SFS_REDIRECT, SFS_STALL, or SFS_STARTED
//!         When SFS_OK is returned, eFlag must be properly set, as follows:
//!         XrdSfsFileExistNo            - path does not exist
//!         XrdSfsFileExistIsFile        - path refers to an  online file
//!         XrdSfsFileExistIsDirectory   - path refers to an  online directory
//!         XrdSfsFileExistIsOffline     - path refers to an offline file
//!         XrdSfsFileExistIsOther       - path is neither a file nor directory
virtual int            exists(const char                *path,
                                    XrdSfsFileExistence &eFlag,
                                    XrdOucErrInfo       &eInfo,
                              const XrdSecEntity        *client = 0,
                              const char                *opaque = 0) = 0;
//! Preapre a file for future processing.