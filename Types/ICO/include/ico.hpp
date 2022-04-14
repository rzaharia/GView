#pragma once

#include "GView.hpp"

namespace GView
{
namespace Type
{
    namespace ICO
    {
#pragma pack(push, 2)
        constexpr uint32 MAGIC_FORMAT_ICO = 0x00010000;
        constexpr uint32 MAGIC_FORMAT_CUR = 0x00020000;

        struct Header
        {
            uint32 magic;
            uint16 count;
        };
        struct IconDirectoryEntry
        {
            uint8 width;
            uint8 height;
            uint8 colorPallette;
            uint8 reserved;
            uint16 colorPlanes;
            uint16 bitsPerPixels;
            uint32 size;
            uint32 offset;
        };
        struct CursorDirectoryEntry
        {
            uint8 width;
            uint8 height;
            uint8 colorPallette;
            uint8 reserved;
            uint16 hotstopX;
            uint16 hotstopY;
            uint32 size;
            uint32 offset;
        };
        union DirectoryEntry
        {
            IconDirectoryEntry ico;
            CursorDirectoryEntry cursor;
        };

#pragma pack(pop) // Back to 4 byte packing.

        class ICOFile : public TypeInterface, public View::ImageViewer::LoadImageInterface
        {
          public:
            bool isIcoFormat;
            
          public:
            Reference<GView::Utils::DataCache> file;
            std::vector<DirectoryEntry> dirs;
          public:
            ICOFile(Reference<GView::Utils::DataCache> file);
            virtual ~ICOFile()
            {
            }

            bool Update();
            void UpdateBufferViewZones(GView::View::BufferViewer::Settings& settings);
            uint32 GetIconsCount() const
            {
                return (uint32) dirs.size();
            }

            std::string_view GetTypeName() override
            {
                return isIcoFormat ? "ICO" : "CURSOR";
            }

            bool LoadImageToObject(Image& img, uint32 index) override;
        };
        namespace Panels
        {
            class Information : public AppCUI::Controls::TabPage
            {
                Reference<GView::Type::ICO::ICOFile> ico;
                Reference<AppCUI::Controls::ListView> general;
                Reference<AppCUI::Controls::ListView> issues;

                void UpdateGeneralInformation();
                void UpdateIssues();
                void RecomputePanelsPositions();

              public:
                Information(Reference<GView::Type::ICO::ICOFile> ico);

                void Update();
                virtual void OnAfterResize(int newWidth, int newHeight) override
                {
                    RecomputePanelsPositions();
                }
            };
            class Directories : public AppCUI::Controls::TabPage
            {
                Reference<GView::Type::ICO::ICOFile> ico;
                Reference<GView::View::WindowInterface> win;
                Reference<AppCUI::Controls::ListView> list;

                void GoToSelectedDirectory();
                void SelectCurrentDirectory();
              public:
                Directories(Reference<GView::Type::ICO::ICOFile> ico, Reference<GView::View::WindowInterface> win);
                bool OnUpdateCommandBar(AppCUI::Application::CommandBar& commandBar) override;
                bool OnEvent(Reference<Control> ctrl, Event evnt, int controlID) override;
                void Update();
            };

        }; // namespace Panels
    }      // namespace ICO
} // namespace Type
} // namespace GView
