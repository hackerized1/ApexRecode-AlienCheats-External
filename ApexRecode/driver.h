#pragma once
#include <WinSock.h>
#include <cstdint>
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <memory>
#include <string_view>
#include <cstdint>
#include <chrono>
#include <vector>
inline DWORD g_pid;
inline uintptr_t g_base;
    namespace driver {
        typedef INT64(*NtUserSetClipboardViewer)(uintptr_t);
        NtUserSetClipboardViewer communicati = nullptr;
        struct _requests {
            uint32_t    src_pid;
            uint64_t    src_addr;
            uint32_t    dst_pid;
            uint64_t    dst_addr;
            size_t        size;
            size_t transfer;
            std::uintptr_t   buffer;
            uint32_t	pid;
            int request_key;
            std::uintptr_t allocation;
        };
        uint32_t g_pid;
        bool core_init() {
            communicati = (NtUserSetClipboardViewer)GetProcAddress(LoadLibraryA("win32u.dll"), "NtUserRemoveInjectionDevice");
            if (!communicati)
            {
                printf("[>] driver cant found...");
                return false;
            }
            return true;
        }
        template<typename Value_T>
        void write(uintptr_t addr, Value_T val) {
            _requests out = { 0 };
            out.request_key = 1335;
            out.pid = g_pid;
            out.dst_pid = GetCurrentProcessId();
            out.src_addr = addr;
            out.size = sizeof(Value_T);
            out.buffer = (uintptr_t)&val;
            uint64_t status = communicati(reinterpret_cast<uintptr_t>(&out));
        }
        uintptr_t get_base_self()
        {
            _requests out = { 0 };
            out.request_key = 1337;
            out.pid = GetCurrentProcessId();
            uint64_t status = communicati(reinterpret_cast<uintptr_t>(&out));
            return out.allocation;
        }
        uintptr_t get_base()
        {
            _requests out = { 0 };
            out.request_key = 1337;
            out.pid = g_pid;
            uint64_t status = communicati(reinterpret_cast<uintptr_t>(&out));
            return out.allocation;
        }
        template<typename Value_T>
        Value_T read(uintptr_t addr, size_t size = sizeof(Value_T))
        {
            Value_T val;
            _requests out = { 0 };
            out.request_key = 1336;
            out.pid = g_pid;
            out.dst_pid = GetCurrentProcessId();
            out.src_addr = addr;
            out.size = size;
            out.buffer = (uintptr_t)&val;
            uint64_t status = communicati(reinterpret_cast<uintptr_t>(&out));
            return val;
        }
    }