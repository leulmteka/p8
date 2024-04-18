#include "sys.h"
#include "stdint.h"
#include "idt.h"
#include "debug.h"
#include "threads.h"
#include "process.h"
#include "machine.h"
#include "ext2.h"
#include "elf.h"
#include "libk.h"
#include "file.h"
#include "heap.h"
#include "shared.h"
#include "kernel.h"
#include "physmem.h"
#include "vmm.h"
#include "filewrapper.h"

int SYS::exec(const char *path, int argc, const char *argv[])
{
    using namespace gheith;

    TCB *me = current();
    Process* MeProcess = me->process;

    if (path == nullptr)
    {
        // Debug::printf("*** HELLO 1\n");
        MeProcess->exit(-1);
        return -1;
    }

    auto nodeForElf = root_fs->find(root_fs->root, path);
    if (nodeForElf == nullptr)
    {
        return -1;
    }

    if (nodeForElf->is_dir())
    {
        // Debug::printf("*** HELLO 3\n");
        MeProcess->exit(-1);
        return -1;
    }

    ElfHeader hdr;

    nodeForElf->read(0, hdr);
    if (hdr.phoff == 0 || hdr.phnum == 0)
    {
        //  Debug::printf("*** HELLO 6\n");
        MeProcess->exit(-1);
        return -1;
    }

    if (hdr.ehsize != sizeof(ElfHeader))
    {
        //   Debug::printf("*** HELLO 7\n");
        MeProcess->exit(-1);
        return -1;
    }

    if (hdr.maigc0 != 0x7F || hdr.magic1 != 'E' || hdr.magic2 != 'L' || hdr.magic3 != 'F')
    {
        //   Debug::printf("*** HELLO 8\n");
        MeProcess->exit(-1);
        return -1;
    }
    if (hdr.cls != 1)
    {
        //   Debug::printf("*** HELLO 9\n");
        MeProcess->exit(-1);
        return -1;
    }
    if (hdr.encoding != 1)
    {
        //  Debug::printf("*** HELLO 10\n");
        MeProcess->exit(-1);
        return -1;
    }
    if (hdr.version != 1)
    {
        //  Debug::printf("*** HELLO 11\n");
        MeProcess->exit(-1);
        return -1;
    }
    if (hdr.type != 2)
    {
        //  Debug::printf("*** HELLO 13\n");
        MeProcess->exit(-1);
        return -1;
    }
    if (hdr.abi != 0)
    {
        //  Debug::printf("*** HELLO 133\n");
        MeProcess->exit(-1);
        return -1;
    }

    uint32_t *sp = (uint32_t*)0xefffe000;

    sp = sp - (argc + 1);
    char **argvs = (char**) sp;

    for (int i = 0; i < argc; i++)
    {
        size_t lengthArg = K::strlen(argv[i]) + 1;
        sp = (uint32_t*)((char*)sp - lengthArg);
        argvs[i] = (char *)sp;
        memcpy(sp, argv[i], lengthArg);
    }
    argvs[argc] = nullptr;

    sp = (uint32_t *)((uint32_t)sp - sizeof(uint32_t));
    *sp = (uint32_t) nullptr;

    sp = (uint32_t *)((uint32_t)sp & ~0x3);
    sp--;
    *sp = (uint32_t)argvs;
    sp--;
    *sp = argc;

    uint32_t entry_point = ELF::load(nodeForElf);
    if (entry_point == (uint32_t)-1)
    {
        //  Debug::printf("*** HELLO 14\n");
        MeProcess->exit(-1);
        return -1;
    }
    // Debug::printf("*** HELLO 15\n");
    switchToUser(entry_point, (uint32_t)sp, 0);
    // Debug::printf("*** HELLO 16\n");
    return -1;
}

bool canAccess(uint32_t address){
    if(address < 0x80000000)
        return false;

    if(address > 0xFFFFFFFF)
        return false;

    if(address >= 0xFEC00000 && address < 0xFEC00000 + 0x1000)
        return false;

    if(address >= 0xFEE00000 && address < 0xFEE00000 + 0x1000)
        return false;

    return true;
}

extern "C" int sysHandler(uint32_t eax, uint32_t *frame)
{
    using namespace gheith;

    auto me = current();

    Process* process = me->process;
    uint32_t *userEsp = (uint32_t *)frame[3];

    uint32_t userPC = frame[0];

    switch (eax)
    {
    case 0:
    {
        uint32_t status = userEsp[1];
        process->exit(status);
        stop();
        return 0;
    }
    case 1: /* write */
    {
        int fd = (int)userEsp[1];
        char *buf = (char*)userEsp[2];
        size_t nbyte = (size_t)userEsp[3];

        if (fd == 0)
        {
            // Debug::printf("*** HELLO 177\n");
            return -1;
        }

        auto f = process->getFile(fd);
        if (f == nullptr)
        {
            //    Debug::printf("*** HELLO 17\n");
            return -1;
        }
        return f->write((void*)buf, nbyte);
    }
    case 2: /* fork */
    {
        int id;
        auto child = process->fork(id);
        if (child == nullptr)
        {
            //   Debug::printf("*** HELLO 18\n");
            return -1;
        }

        thread(child, [userEsp, userPC]
               { switchToUser(userPC, (uint32_t)userEsp, 0); });

        if (process == child)
        {
            return 0;
        }
        return id;
    }
    case 3: /* sem */
    {
        uint32_t init = userEsp[1];
        return process->newSemaphore(init);
    }
    case 4: /* up */
    {
        int semaphoreID = (int)userEsp[1];
        auto semaphore = process->getSemaphore(semaphoreID);
        if (semaphore == nullptr)
        {
            //  Debug::printf("*** HELLO 19\n");
            return -1;
        }
        semaphore->up();
        return 0;
    }
    case 5: /* down */
    {
        int semaphoreID = (int)userEsp[1];
        auto semaphore = process->getSemaphore(semaphoreID);
        if (semaphore == nullptr)
        {
            //  Debug::printf("*** HELLO 20\n");
            return -1;
        }
        semaphore->down();
        return 0;
    }
    case 6: /* close */
    {
        int id = (int)userEsp[1];
        return process->close(id);
    }
    case 7: /* shutdown */
    {
        // Debug::printf("*** HELLO 21\n");
        Debug::shutdown();
        //  Debug::printf("*** HELLO 22\n");
        return -1;
    }
    case 8: /* wait */
    {
        int id = (int)userEsp[1];
        uint32_t *statusPtr = (uint32_t*)userEsp[2];

        if (!canAccess((uint32_t)statusPtr))
        {
            //       Debug::printf("*** HELLO 23\n");
            return -1;
        }
        
        return process->wait(id, statusPtr);
    }
    case 9: /* execl */
    {
        char *path = (char*)userEsp[1];

        if (path == nullptr || !canAccess((uint32_t)path))
        {
            //     Debug::printf("*** HELLO 24\n");
            return -1;
        }

        uint32_t argc = 0;
        while (userEsp[2 + argc] != (uint32_t) nullptr)
        {
            char *arg = (char*)userEsp[2 + argc];
            if (!canAccess((uint32_t)arg))
            {
                //       Debug::printf("*** HELLO 25\n");
                return -1;
            }
            argc++;
        }

        const char **argv = new const char *[1 + argc];
        for (uint32_t i = 0; i < argc; i++)
        {
            char *argForUser = (char*)userEsp[2 + i];
            size_t length = 1 + K::strlen(argForUser);
            argv[i] = new char[length];
            memcpy((void *)argv[i], (const void*)argForUser, length);
        }
        argv[argc] = nullptr;

        Node* nodeForElf = root_fs->find(root_fs->root, path);
        if (nodeForElf == nullptr || !nodeForElf->is_file())
        {
            //   Debug::printf("*** HELLO 26\n");
            delete[] argv;
            return -1;
        }

        ElfHeader hdr;
        nodeForElf->read(0, hdr);
        if (hdr.maigc0 != 0x7F || hdr.magic1 != 'E' || hdr.magic2 != 'L' || hdr.magic3 != 'F')
        {
            // Debug::printf("*** HELLO 27\n");
            return -1;
        }

        if (hdr.ehsize != sizeof(ElfHeader))
        {
            // Debug::printf("*** HELLO 6\n");
            //  Debug::printf("*** HELLO 28\n");
            return -1;
        }

        int output = SYS::exec(path, argc, argv);
        delete[] argv;
        return output;
    }
    case 10: /* open */
    {
        char *path = (char*)userEsp[1];
        if (path == nullptr || !canAccess( (uint32_t)path))
        {
            return -1;
        }

        Node* files = root_fs->find(root_fs->root, path);
        if (files == nullptr)
        {
            //   Debug::printf("*** HELLO 30\n");
            current()->process->exit(-1);
            return -1;
        }

        while (files->is_symlink())
        {
            char *buf = new char[files->size_in_bytes() + 1];
            files->get_symbol(buf);
            buf[files->size_in_bytes()] = '\0';
            files = root_fs->find(root_fs->root, buf);
        }

        File* fileWrapper{new FileWrapper(files)};
        auto MeProcess = current()->process;
        int fd = MeProcess->setFile(fileWrapper);
        return fd;
    }

    case 11: /* len */
    {
        int fd = (int)userEsp[1];

        if (fd <= 2)
        {
            //  Debug::printf("*** HELLO 33\n");
            return -1;
        }

        auto process = current()->process;
        auto file = process->getFile(fd);

        if (file == nullptr)
        {
            // Debug::printf("*** HELLO 25\n");
            //  Debug::printf("*** HELLO 34\n");
            return -1;
        }

        return file->size();
    }

    case 12: /* read */
    {
        int fd = (int)userEsp[1];
        char *buf = (char *)userEsp[2];
        size_t nbyte = (size_t)userEsp[3];

        auto process = current()->process;
        if (fd == 1)
        {
            //  Debug::printf("*** HELLO 35\n");
            return -1;
        }

        if(!canAccess((uint32_t)buf) || !canAccess( (uint32_t)buf + (uint32_t)nbyte      )){
            process->exit(-1);
            return -1;
        }
        
        auto file = process->getFile(fd);
        if (file == nullptr)
        {
            // Debug::printf("*** HELLO 37\n");
            process->exit(-1);
            return -1;
        }
        return file->read(buf, nbyte);
    }

    case 13: /* seek */
    {
        int fd = (int)userEsp[1];
        auto process = current()->process;
        if (fd == 1)
        {
          // Debug::printf("*** HELLO 38\n");
            process->exit(-1);
            return -1;
        }

        auto file = process->getFile(fd);
        if (file == nullptr)
        {
            // Debug::printf("*** HELLO 39\n");
            process->exit(-1);
            return -1;
        }

        off_t offset = (off_t)userEsp[2];
        return file->seek(offset);
    }
    default:
        Debug::printf("*** 1000000000 unknown system call %d\n", eax);
        // process->exit(-1);
        // stop();
        return -1;
    }
}

void SYS::init(void)
{
    IDT::trap(48, (uint32_t)sysHandler_, 3);
}