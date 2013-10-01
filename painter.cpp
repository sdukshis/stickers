#include <iostream>

#include "PDFWriter.h"
#include "PDFEmbedParameterTypes.h"
#include "PageContentContext.h"
#include "PDFPage.h"
#include "PDFDocumentCopyingContext.h"

#include "painter.h"

using std::cerr;
using std::endl;

static const int SOCLE_WIDTH  = 156;
static const int SOCLE_HEIGHT = 100;
static const std::string FONT_PATH = "fonts/";
static const std::string LOGO_PATH = "logo/";
static const std::string ICON_PATH = "icon/";

ObjectIDType getPic(PDFWriter& pdfWriter, const std::string& filepath) {
    EStatusCode status;
    EStatusCodeAndObjectIDTypeList result = 
        pdfWriter.CreateFormXObjectsFromPDF(filepath, PDFPageRange(),
                                            ePDFPageBoxMediaBox);
    if (result.first != eSuccess) {
        cerr << "Error open logo file '" << filepath << "'" << endl;
        status = eFailure;
        return NULL;
    }
    ObjectIDTypeList::iterator it = result.second.begin();
    return *it;    
}


int socle_painter(const Sticker& sticker) {
    PDFWriter pdfWriter;
    EStatusCode status = eSuccess;

    do {
        status = pdfWriter.StartPDF(sticker.toString() + ".pdf", ePDFVersion13);
        if (status != eSuccess) {
            cerr << "Error startPdf" << endl;
            break;
        }

        ObjectIDType Logo = getPic(pdfWriter, LOGO_PATH + sticker.getLogo() + ".pdf");

        PDFPage* page = new PDFPage();
        page->SetMediaBox(PDFRectangle(0, 0, SOCLE_WIDTH, SOCLE_HEIGHT));

        PageContentContext* contentContext = 
            pdfWriter.StartPageContentContext(page);

        PDFUsedFont* FreeSans = 
            pdfWriter.GetFontForFile(FONT_PATH + "FreeSans.ttf");
        if (!FreeSans) {
            cerr << "Error open font file '" << "FreeSans.ttf" << "'" << endl;
            status = eFailure;
            break;
        }

        // Draw Logo
        contentContext->q();
        contentContext->cm(1, 0, 0, 1, 1, 75);
        contentContext->Do(
            page->GetResourcesDictionary().AddFormXObjectMapping(Logo));
        contentContext->Q();

        // Draw Article
        contentContext->BT();
        contentContext->Tf(FreeSans,16);
        contentContext->TD(1, 62);
        contentContext->Tj("Арт:" + sticker.getArticle());
        contentContext->ET();

        // Draw Type
        contentContext->BT();
        contentContext->Tf(FreeSans,16);
        contentContext->TD(1, 42);
        contentContext->Tj(sticker.getType());
        contentContext->ET();

        // Draw Voltage
        contentContext->BT();
        contentContext->Tf(FreeSans,16);
        contentContext->TD(1, 22);
        contentContext->Tj(sticker.getVoltage());
        contentContext->ET();

        // Draw IP
        contentContext->BT();
        contentContext->Tf(FreeSans,20);
        contentContext->TD(1, 4);
        contentContext->Tj(sticker.getIP());
        contentContext->ET();

        // Draw  icons

        ObjectIDType pic01 = getPic(pdfWriter, ICON_PATH + "01.pdf");
        contentContext->q();
        contentContext->cm(1, 0, 0, 1, 20, 4);
        contentContext->Do(
            page->GetResourcesDictionary().AddFormXObjectMapping(pic01));
        contentContext->Q();

        status = pdfWriter.EndPageContentContext(contentContext);
        if (status != eSuccess) {
            cerr << "Error EndPageContentContext" << endl;
            break;
        }

        status = pdfWriter.WritePageAndRelease(page);
        if (status != eSuccess) {
            cerr << "Error WritePageAndRelease" << endl;
            break;
        }

        status = pdfWriter.EndPDF();
        if (status != eSuccess) {
            cerr << "Error EndPDF" << endl;
            break;
        }

    } while (false);

    return status != eSuccess;
}

int cup_painter(const Sticker& sticker) {
    EStatusCode status = eSuccess;

    do {

    } while (false);

    return 0;
}