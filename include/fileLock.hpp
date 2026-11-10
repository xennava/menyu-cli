#pragma once

#include <filesystem>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
typedef HANDLE LockHandle;
#define INVALID_LOCK INVALID_HANDLE_VALUE
#else
#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>
typedef int LockHandle;
#define INVALID_LOCK -1
#endif

class FileLock {
private:
  LockHandle hLock;
  std::string lockPath;
  bool isLocked;

public:
  FileLock()
      : hLock(INVALID_LOCK),
        lockPath(
            (std::filesystem::temp_directory_path() / "fjk.lock").string()),
        isLocked(false) {}
  FileLock(const std::string &path)
      : hLock(INVALID_LOCK), lockPath(path), isLocked(false) {}

  ~FileLock() { release(); }

  bool acquire() {
#if defined(_WIN32) || defined(_WIN64)
    hLock = CreateFileA(
        lockPath.c_str(), GENERIC_READ | GENERIC_WRITE,
        0, // 0 artinya EXCLUSIVE access.
        NULL,
        CREATE_ALWAYS, // Buat baru jika belum ada, timpa jika sudah ada
        FILE_ATTRIBUTE_NORMAL |
            FILE_FLAG_DELETE_ON_CLOSE, // Otomatis hapus saat closed
        NULL);

    if (hLock == INVALID_HANDLE_VALUE) {
      return false; // Gagal mengunci karena file sedang dikunci proses lain
    }
#else
    // Linux / macOS: Buka file biasa
    hLock = open(lockPath.c_str(), O_RDWR | O_CREAT, 0666);
    if (hLock == -1) {
      return false;
    }

    // Terapkan eksklusif lock (LOCK_EX) tanpa memblokir proses (LOCK_NB)
    if (flock(hLock, LOCK_EX | LOCK_NB) != 0) {
      close(hLock);
      hLock = INVALID_LOCK;
      return false; // Sedang dikunci proses lain
    }
#endif
    isLocked = true;
    return true;
  }

  // Melepas kunci
  void release() {
    if (!isLocked)
      return;

#if defined(_WIN32) || defined(_WIN64)
    if (hLock != INVALID_HANDLE_VALUE) {
      CloseHandle(hLock); // Karena flag FILE_FLAG_DELETE_ON_CLOSE, file
                          // otomatis terhapus
      hLock = INVALID_HANDLE_VALUE;
    }
#else
    if (hLock != -1) {
      flock(hLock, LOCK_UN); // Lepas lock
      close(hLock);
      unlink(lockPath.c_str()); // Hapus file .tmp
      hLock = INVALID_LOCK;
    }
#endif
    isLocked = false;
  }
};
