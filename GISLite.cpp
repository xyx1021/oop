#include "GISLite.h"
#include <QDebug>

QVector<Geometry*> g;
QVector<Layer*> LayerList;
View* view = nullptr;
Layer* currentLayer = nullptr;
//QVector<int> selectViewSet;
QVector<View*> selectSet;
int selectSetIndex = 0;
QWidget* MainWindow;
//int num_layer = 0;
MouseMode* currentMode = nullptr;
QToolBar* toolbar,* addbar,* editbar,* stylebar;


GISLite::GISLite(QWidget *parent)
    : QMainWindow(parent),ui(new Ui::GISLiteClass)
{
    ui->setupUi(this);
    //CoengHau GeiLuk & BeiLai CitDing 
    MainWindow = this;
    windowScaletime = 1;
    //MakJing LauLaam MouSik
    setState(new BrowseMode());
    //SanGin CoiDaanLaan & Zyu GungGeoiLaan & TimGaa GungGeoiLaan & BinCap GungGeoiLaan & LitBiu
    menuBar = new QMenuBar(this);
    toolbar = new QToolBar(this);
    addbar = new QToolBar(this);
    editbar = new QToolBar(this);
    stylebar = new QToolBar(this);
    listWidget = new QListWidget(this);  
    listWidget->setGeometry(this->width() - 200, 20 + toolbar->height() + menuBar->height(), 200, this->height() - 40 - toolbar->height() - menuBar->height());
    listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    //KauGin CoiDaan 1
    QMenu* menuA = new QMenu("File");
    menuA->addAction("New");
    menuA->addAction("Open");
    menuA->addAction("Import");
    menuA->addAction("Save");
    menuA->addAction("Save as");
    menuBar->addMenu(menuA);
    //KauGin CoiDaan 2
    QMenu* menuB = new QMenu("Edit");
    menuB->addAction("Start Editing");
    menuB->addAction("End and Exit");
    menuBar->addMenu(menuB);
    //Zoeng CoiDaan GaaJap CoengHau
    setMenuBar(menuBar);
    
    //CitDing Zyu GungGeoiLaan JoengSik
    QAction* action0 = toolbar->addAction(QIcon(":/GISLite/icons/blue-handcursor-32.png"), "Drag");
    QAction* action1 = toolbar->addAction(QIcon(":/GISLite/icons/cursor-32.png"), "Select");
    QAction* action2 = toolbar->addAction(QIcon(":/GISLite/icons/plus-32.png"), "Add");
   // toolbar->addSeparator();
    QAction* action4 = toolbar->addAction(QIcon(":/GISLite/icons/backarrow-32.png"), "Undo");
    QAction* action5 = toolbar->addAction(QIcon(":/GISLite/icons/forwardarrow-32.png"), "Redo");
    QAction* action6 = toolbar->addAction(QIcon(":/GISLite/icons/earth-32.png"), "Reposition");    

    //SanGin MingLing
    CommandBrowse* CB = new CommandBrowse;
    CommandSelect* CS = new CommandSelect;
    CommandAdd* CA = new CommandAdd;
    CommandAddPoint* CAP = new CommandAddPoint;
    CommandAddLine* CAL = new CommandAddLine;
    CommandAddPolyline* CAPL = new CommandAddPolyline;
    CommandAddRectangle* CAR = new CommandAddRectangle;
    CommandAddPolygon* CAPG = new CommandAddPolygon;
    CommandAddCircle* CAC = new CommandAddCircle;
    CommandAddSector* CAS = new CommandAddSector;
    //CommandStyle* CST = new CommandStyle;
    CommandBorderColor* CBC = new CommandBorderColor;
    CommandBorderStyle* CBS = new CommandBorderStyle;
    CommandFillColor* CFC = new CommandFillColor;
    CommandFillStyle* CFS = new CommandFillStyle;
    CommandWidth* CW = new CommandWidth;
    CommandEdit* CD = new CommandEdit;
    CommandTranslate* CT = new CommandTranslate;
    CommandRotate* CR = new CommandRotate;
    CommandScale* CE = new CommandScale;

    //CitDing TimGaa GungGeoiLaan JoengSik
    QAction* action2a = addbar->addAction(QIcon(":/GISLite/icons/point-32.png"), "Point");
    QAction* action2b = addbar->addAction(QIcon(":/GISLite/icons/line-32.png"), "Line");
    QAction* action2c = addbar->addAction(QIcon(":/GISLite/icons/polyline-32.png"), "Polyline");
    QAction* action2d = addbar->addAction(QIcon(":/GISLite/icons/rectangle-32.png"), "Rectangle");
    QAction* action2e = addbar->addAction(QIcon(":/GISLite/icons/polygon-32.png"), "Polygon");
    QAction* action2f = addbar->addAction(QIcon(":/GISLite/icons/circle-32.png"), "Circle");
    QAction* action2g = addbar->addAction(QIcon(":/GISLite/icons/t-32.png"), "Sector");
    QAction* action2h = addbar->addAction(QIcon(":/GISLite/icons/sector-32.png"), "Txt");


    //CitDing BinCap GungGeoiLaan JoengSik
    QAction* action7a = editbar->addAction(QIcon(":/GISLite/icons/editpoint-32.png"), "Edit Point");
    QAction* action7b = editbar->addAction(QIcon(":/GISLite/icons/move-32.png"), "Move");
    QAction* action7c = editbar->addAction(QIcon(":/GISLite/icons/rotate-32.png"), "Rotate");
    QAction* action7d = editbar->addAction(QIcon(":/GISLite/icons/enlarge-32.png"), "Scale");

    QAction* action8a = stylebar->addAction("Border Color");
    QAction* action8b = stylebar->addAction("Border Style");
    QAction* action8c = stylebar->addAction("Fill Color");
    QAction* action8d = stylebar->addAction("Fill Style");
    QAction* action8e = stylebar->addAction("Width");

    //Zoeng GungGeoiLaan GaaJap CoengHau
    addToolBar(toolbar);
    addToolBar(addbar);addbar->setVisible(false);
    addToolBar(editbar);editbar->setVisible(true);
    addToolBar(stylebar); stylebar->setVisible(true);

    //Signal And Slot
 
    //BongDing GungGeoiLaan OnGin Y MingLing
    connect(action0, SIGNAL(triggered()), CB, SLOT(Invoke()));
    connect(action1, SIGNAL(triggered()), CS, SLOT(Invoke()));
    connect(action2, SIGNAL(triggered()), CA, SLOT(Invoke()));
    connect(action2a, SIGNAL(triggered()), CAP, SLOT(Invoke()));
    connect(action2b, SIGNAL(triggered()), CAL, SLOT(Invoke()));
    connect(action2c, SIGNAL(triggered()), CAPL, SLOT(Invoke()));
    connect(action2d, SIGNAL(triggered()), CAR, SLOT(Invoke()));
    connect(action2e, SIGNAL(triggered()), CAPG, SLOT(Invoke()));
    connect(action2f, SIGNAL(triggered()), CAC, SLOT(Invoke()));
    connect(action2g, SIGNAL(triggered()), CAS, SLOT(Invoke()));


    connect(action6, &QAction::triggered, this, &GISLite::onRepositionClicked);

    connect(action7a, SIGNAL(triggered()), CD, SLOT(Invoke()));
    connect(action7b, SIGNAL(triggered()), CT, SLOT(Invoke()));
    connect(action7c, SIGNAL(triggered()), CR, SLOT(Invoke()));
    connect(action7d, SIGNAL(triggered()), CE, SLOT(Invoke()));

    connect(action8a, SIGNAL(triggered()), CBC, SLOT(Invoke()));
    connect(action8b, SIGNAL(triggered()), CBS, SLOT(Invoke()));
    connect(action8c, SIGNAL(triggered()), CFC, SLOT(Invoke()));
    connect(action8d, SIGNAL(triggered()), CFS, SLOT(Invoke()));
    connect(action8e, SIGNAL(triggered()), CW, SLOT(Invoke()));
    //connect(&selectSet, &QVector<int>::valueChanged, &command, &Command::Invoke);
    //LitBiu DimGik SiGin
    connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onItemClicked(QListWidgetItem*)));
    connect(listWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(on_listWidget_customContextMenuRequested(const QPoint&)));

    //CoiDaan DimGik SiGin
    connect(menuA->actions().at(1), &QAction::triggered, this, &GISLite::openFile);
    connect(menuA->actions().at(2), &QAction::triggered, this, &GISLite::importFile);
    connect(menuA->actions().at(3), &QAction::triggered, this, &GISLite::saveFile); 


    centralWidget()->setMouseTracking(true);

    QVector<Geometry*> All;
    Layer* newlayer;
    newlayer = new Layer("All");
    LayerList.append(newlayer);
    LayerList.last()->setName("All");

    QListWidgetItem* item = new QListWidgetItem("ALL");
    listWidget->addItem(item);

}

GISLite::~GISLite()
{}

void GISLite::getView(QVector<Geometry*> geom, int j)
{    
    if (!strcmp(geom[j]->getname(), "POINT"))
        view = new PointView(*(CPoint*)geom[j]);
    else if (!strcmp(geom[j]->getname(), "LINESTRING"))
        view = new LineView(*((CLine*)geom[j]));
    else if (!strcmp(geom[j]->getname(), "POLYGON"))
        view = new PolygonView(*((CPolygon*)geom[j]));
    else if (!strcmp(geom[j]->getname(), "RECTANGLE"))
        view = new RectangleView(*((CRectangle*)geom[j]));
    else if (!strcmp(geom[j]->getname(), "CIRCLE"))
        view = new CircleView(*((CCircle*)geom[j]));
    else if (!strcmp(geom[j]->getname(), "SECTOR"))
        view = new SectorView(*((CSector*)geom[j]));
}

void GISLite::openFile()
{
    QString filter = tr("GISLite File (*.gisk);");
    QStringList FileName = QFileDialog::getOpenFileNames(this, tr("Open File"), "", filter);
    if (FileName.isEmpty())
    {
        return;
    }
    FileData F;
    for (int i = 0; i < FileName.size(); i++)
    {
        QVector<Geometry*> newQG;
        Layer* newlayer;
        newlayer = new Layer(FileName[i]);
        LayerList.append(newlayer);
        LayerList.last()->setName("Layer" + QString::number(LayerList.count() - 1));

        if (FileName[i].right(4) == "gisk")
        {
            F.getGisk(FileName[i], newQG);
        }

        for (int j = 0; j < newQG.size(); j++)
        {
            qDebug() << "The geometry" << newQG[j]->getname() << "read finished" << endl;
            getView(newQG, j);
            LayerList.last()->getVector().push_back(view);
        }
        QListWidgetItem* item = new QListWidgetItem(QString::fromStdString("Layer" + std::to_string(LayerList.count()-1)));
        listWidget->addItem(item);
    }
    repaint();
}

void GISLite::importFile()
{
    QString filter = tr("Text Files (*.txt);;ShapeFile (*.shp);;All Files (*)");
    QStringList FileName = QFileDialog::getOpenFileNames(this, tr("Open File"), "", filter);
    if (FileName.isEmpty())
    {
        return;
    }
    FileData F;
    for (int i = 0; i < FileName.size(); i++)
    {
        QVector<Geometry*> newQG;
        Layer* newLayer;
        newLayer = new Layer(FileName[i]);
        LayerList.push_back(newLayer);
        LayerList.last()->setName("Layer" + QString::number(LayerList.count() - 1));

        if (FileName[i].right(3) == "txt")
        {

        }
        else if (FileName[i].right(3) == "shp")
        {
            F.getShp(FileName[i], newQG);
        }
        for (int j = 0; j < newQG.size(); j++)
        {
            qDebug() << "The geometry" << newQG[j]->getname() << "read finished" << endl;
            getView(newQG, j);
            LayerList.last()->getVector().push_back(view);
        }
        QListWidgetItem* item = new QListWidgetItem(QString::fromStdString("Layer" + std::to_string(LayerList.count()-1)));
        listWidget->addItem(item);
    }
    repaint();
}

void GISLite::saveFile()
{
    QString savePath = QFileDialog::getSaveFileName(nullptr, "Save File", "", "GISLite File (*.gisk)");

    if (savePath.isEmpty())
    {
		return;
	}
    else
    {		
        FileData F;
        F.saveData(savePath, g);
	}
}

void GISLite::onItemClicked(QListWidgetItem* item)
{
    selectSetIndex = listWidget->row(item);
    selectSet.clear();
    selectSet.append(LayerList[selectSetIndex]->getVector());
    currentLayer = LayerList[selectSetIndex];
    repaint();
}

void GISLite::onRepositionClicked()
{
    BBox BBoxSet;
    BBoxSet.xmax = -std::numeric_limits<double>::infinity(); BBoxSet.xmin = std::numeric_limits<double>::infinity(); BBoxSet.ymax = -std::numeric_limits<double>::infinity(); BBoxSet.ymin = std::numeric_limits<double>::infinity();
    for (auto p : selectSet)
    {
        if (p->get_box().xmax > BBoxSet.xmax)BBoxSet.xmax = p->get_box().xmax;
        if (p->get_box().xmin < BBoxSet.xmin)BBoxSet.xmin = p->get_box().xmin;
        if (p->get_box().ymax > BBoxSet.ymax)BBoxSet.ymax = p->get_box().ymax;
        if (p->get_box().ymin < BBoxSet.ymin)BBoxSet.ymin = p->get_box().ymin;
        BBoxSet.center = Point((BBoxSet.xmax + BBoxSet.xmin) / 2, (BBoxSet.ymax + BBoxSet.ymin) / 2);
    }

    int scalex = this->width() / (BBoxSet.xmax - BBoxSet.xmin);   
    int scaley = this->height() / (BBoxSet.ymax - BBoxSet.ymin); 

    windowScaletime = scalex < scaley ? scalex : scaley;
    windowScaletime /= 2;

    windowTranslateAccumalation.x = this->width() / (2 * windowScaletime) - 0.5 * BBoxSet.xmin - 0.5 * BBoxSet.xmax-100/windowScaletime;
    windowTranslateAccumalation.y = this->height() / (2 * windowScaletime) - 0.5 * BBoxSet.ymin - 0.5 * BBoxSet.ymax;

    repaint();
}


void GISLite::on_listWidget_customContextMenuRequested(const QPoint& pos)
{
    int rightSelectSetIndex = listWidget->row(listWidget->itemAt(pos));
    QPoint globalPos = listWidget->mapToGlobal(pos);
    QMenu menu;

    menu.addAction("Add a layer");

    QMenu* mergeMenu = menu.addMenu("Merge to");
    for (int i= 1;i<LayerList.count();i++)
    {
        if(i!= rightSelectSetIndex)
	    mergeMenu->addAction("Merge with " + LayerList[i]->getName());
    }

    menu.addAction("Set name");
    menu.addAction("Delete");


    QAction* selectedItem = menu.exec(globalPos); 

    if (selectedItem)
    {
       /* if (selectedItem->text().left(10) == "Merge with")
        {
            for (int i = 0; i < LayerList.count(); i++)
            {
                if(LayerList[i]->getName()== selectedItem->text().mid(11))
				{
					LayerList[i]->getVector().append(LayerList[rightSelectSetIndex]->getVector());
                    LayerList.erase(LayerList.begin() + rightSelectSetIndex);
                    listWidget->takeItem(rightSelectSetIndex);
					break;
				}
            }            
        }*/
         if (selectedItem->text() == "Set name")
        {
		    bool ok;
			QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),tr("Layer Name:"), QLineEdit::Normal,"", &ok);
			if (ok && !text.isEmpty())
			{
				LayerList[rightSelectSetIndex]->setName(text);
				listWidget->item(rightSelectSetIndex)->setText(text);
			}
		}
		else if (selectedItem->text() == "Delete")
		{
			LayerList.erase(LayerList.begin() + rightSelectSetIndex);
			listWidget->takeItem(rightSelectSetIndex);
        }
        else if (selectedItem->text() == "Add a layer")
        {
            QVector<Geometry*> newQG;
            Layer* newLayer;
            newLayer = new Layer();
            LayerList.push_back(newLayer);
            LayerList.last()->setName("Layer" + QString::number(LayerList.count() - 1));
            QListWidgetItem* item = new QListWidgetItem(QString::fromStdString("Layer" + std::to_string(LayerList.count() - 1)));
            listWidget->addItem(item);
        }
    }
    if (selectSetIndex >= LayerList.count())
    {
        selectSetIndex = LayerList.count() - 1;
    }
    repaint();
}