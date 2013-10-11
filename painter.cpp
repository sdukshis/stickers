#include <iostream>
#include <sstream>


#include "PDFWriter.h"
#include "PDFEmbedParameterTypes.h"
#include "PageContentContext.h"
#include "PDFPage.h"
#include "PDFDocumentCopyingContext.h"

#include "painter.h"

using std::cerr;
using std::endl;
using std::string;

static const int SOCLE_WIDTH  = 156;
static const int SOCLE_HEIGHT = 100;
static const int CUP_WIDTH = 156;
static const int CUP_HEIGHT = 29;
static const std::string FONT_PATH = "../fonts/";
static const std::string LOGO_PATH = "../logo/";
static const std::string ICON_PATH = "../icon/";

ObjectIDType getPic(PDFWriter& pdfWriter, const std::string& filepath) {
    PDFDocumentCopyingContext* result = 
        pdfWriter.CreatePDFCopyingContext(filepath);
    if (!result) {
        cerr << "Error open file '" << filepath << "'" << endl;
        return 0;
    }
    EStatusCodeAndObjectIDType resultFirst = result->CreateFormXObjectFromPDFPage(0,ePDFPageBoxMediaBox);
    if(resultFirst.first != eSuccess) {
        return 0;
    }

    return resultFirst.second;    
}


int cup_painter(const Sticker& sticker, const string& output) {
    PDFWriter pdfWriter;
    EStatusCode status = eSuccess;

    do {
        status = pdfWriter.StartPDF(output + sticker.toString() + string(" Cup.pdf"), ePDFVersion15);
        if (status != eSuccess) {
            cerr << "Error startPdf" << endl;
            break;
        }

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

        std::stringstream led_type;
        led_type << sticker.getLedType() << ".pdf";
        std::stringstream protection;
        protection << sticker.getProtection() << ".pdf";
        std::stringstream flame;
        flame << sticker.getFlame() << ".pdf";

        ObjectIDType Logo = getPic(pdfWriter, LOGO_PATH + sticker.getLogo() + ".pdf");
        ObjectIDType picFlame = getPic(pdfWriter, ICON_PATH + flame.str());
        ObjectIDType picType = getPic(pdfWriter, ICON_PATH + led_type.str());
        ObjectIDType picProtection = getPic(pdfWriter, ICON_PATH + protection.str());
        ObjectIDType RST = getPic(pdfWriter, ICON_PATH + "RST.pdf");

        // Draw Logo
        contentContext->q();
        contentContext->cm(1, 0, 0, 1, 1, 75);
        contentContext->Do(
            page->GetResourcesDictionary().AddFormXObjectMapping(Logo));
        contentContext->Q();

        // Draw icons
        contentContext->q();
        contentContext->cm(0.1, 0, 0, 0.1, 35, 2);
        contentContext->Do(
           page->GetResourcesDictionary().AddFormXObjectMapping(picFlame));
        contentContext->Q();

        contentContext->q();
        contentContext->cm(0.1, 0, 0, 0.1, 61, 2);
        contentContext->Do(
           page->GetResourcesDictionary().AddFormXObjectMapping(picProtection));
        contentContext->Q();

        contentContext->q();
        contentContext->cm(0.1, 0, 0, 0.1, 87, 2);
        contentContext->Do(
           page->GetResourcesDictionary().AddFormXObjectMapping(picType));
        contentContext->Q();

        contentContext->q();
        contentContext->cm(0.1, 0, 0, 0.1, 113, 2);
        contentContext->Do(
           page->GetResourcesDictionary().AddFormXObjectMapping(RST));
        contentContext->Q();

        // Draw Article
        contentContext->BT();
        contentContext->Tf(FreeSans, 16);
        contentContext->TD(1, 62);
        contentContext->Tj("Арт:");
        contentContext->ET();

        contentContext->BT();
        contentContext->Tf(FreeSans, 14);
        contentContext->TD(35, 62);
        contentContext->Tz(70);
        contentContext->Tj(sticker.getArticle());
        contentContext->ET();

        // Draw Type
        std::stringstream type;
        type << sticker.getType() << "x" << sticker.getNumber();
        contentContext->BT();
        contentContext->Tf(FreeSans,16);
        contentContext->TD(0, 42);
        contentContext->Tj(type.str());
        contentContext->ET();

        // Draw Voltage
        contentContext->BT();
        contentContext->Tf(FreeSans,16);
        contentContext->TD(1, 25);
        contentContext->Tj(sticker.getVoltage());
        contentContext->ET();

        // Draw IP
        contentContext->BT();
        contentContext->Tf(FreeSans,20);
        contentContext->TD(0, 4);
        contentContext->Tj(sticker.getIP());
        contentContext->ET();


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

int socle_painter(const Sticker& sticker, const string& output) {
    PDFWriter pdfWriter;
    EStatusCode status = eSuccess;

    do {
        status = pdfWriter.StartPDF(output + sticker.toString() + std::string(" Socle.pdf"), ePDFVersion15);
        if (status != eSuccess) {
            cerr << "Error startPdf" << endl;
            break;
        }

        PDFPage* page = new PDFPage();
        page->SetMediaBox(PDFRectangle(0, 0, CUP_WIDTH, CUP_HEIGHT));

        PageContentContext* contentContext = 
            pdfWriter.StartPageContentContext(page);


        PDFUsedFont* FreeSans = 
            pdfWriter.GetFontForFile(FONT_PATH + "FreeSans.ttf");
        if (!FreeSans) {
            cerr << "Error open font file '" << "FreeSans.ttf" << "'" << endl;
            status = eFailure;
            break;
        }

         
   
        // Draw Type
        std::stringstream type;
        type << sticker.getType() << "x" << sticker.getNumber();
        type << " " << sticker.getVoltage();
        contentContext->BT();
        contentContext->Tf(FreeSans, 11);
        contentContext->TD(0, 10);
        contentContext->Tj(type.str());
        contentContext->ET();


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