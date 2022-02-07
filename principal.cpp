#include "principal.h"
#include "ui_principal.h"

#include <QDebug>

Principal::Principal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Principal)
{
    ui->setupUi(this);
    // Instancia del objeto imagen
    m_imagen = new QImage(this->size(),QImage::Format_ARGB32_Premultiplied);
    // Rellenar de blanco
    m_imagen->fill(Qt::white);
    // Instanciar el objeto painter
    m_painter = new QPainter(m_imagen);
    // Instanciar el ancho
    m_ancho = DEFAULT_ANCHO;
    // Opcion por defecto
    m_opcion = 1;


    //Establecer el idioma que va a contener el comboBox
    ui->inIdiomas->addItems(QStringList()<< "Español" << "Portugues" << "Catalan");
    //Conectar la señal del combo box
    connect(ui->inIdiomas, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
            [=](const QString &str1){
        // Dependiendo del idioma seleccionado, carga el archivo de traducción
        if(str1=="Catalan"){
            m_traduccion.load(":/Paint_en.qm"+str1,".");
        }
        else if(str1=="Portugues"){
            m_traduccion.load(":/Paint_tu.qm"+str1,".");
        }
        else if(str1=="Español"){
            m_traduccion.load("paint_es.ts"+str1,",");
        }
        // Si es diferente de español, se instala la traducción en TODA la aplicación
        if(str1!="Español"){
            qApp->installTranslator(&m_traduccion);
        }
    });
}

Principal::~Principal()
{
    delete ui;
}

void Principal::paintEvent(QPaintEvent *event)
{
    // Crear el painter de la ventana principal
    QPainter painter(this);
    // Dibujar la imagen
    painter.drawImage(0,0, *m_imagen);
    // Aceptar el evento
    event->accept();
}

void Principal::mousePressEvent(QMouseEvent *event)
{
    if(m_opcion == 1){
        m_ptInicial = event->pos();
    }else if(m_opcion == 2){
        if(m_configuracion){
            m_ptInicial = event->pos();
        }else{
            //Dibujar la linea
            m_ptFinal = event->pos();
            QPen pincel;
            pincel.setWidth(m_ancho);
            pincel.setColor(m_color);
            m_painter->setPen(pincel);
            m_painter->drawLine(m_ptInicial, m_ptFinal);
            update();
        }
    }else if(m_opcion == 3){
        if(m_configuracion){
            m_ptInicial = event->pos();
        }else{
            m_ptFinal = event->pos();
            //Dibujar rectangulo
            int ancho = m_ptFinal.x() - m_ptInicial.x();
            int alto = m_ptFinal.y() - m_ptInicial.y();
            QPen pincel;
            pincel.setWidth(m_ancho);
            pincel.setColor(m_color);
            m_painter->setPen(pincel);
            m_painter->drawRect(m_ptInicial.x(), m_ptInicial.y(), ancho, alto);
            update();
        }
    }else if(m_opcion == 4){
        if(m_configuracion){
            m_ptInicial = event->pos();
        }else{
            //Dibujar el circulo
            m_ptFinal = event->pos();
            int ancho = m_ptFinal.x() - m_ptInicial.x();
            int alto = m_ptFinal.y() - m_ptInicial.y();
            QPen pincel;
            pincel.setWidth(m_ancho);
            pincel.setColor(m_color);
            m_painter->setPen(pincel);
            QRectF rectangle(m_ptInicial.x(),m_ptInicial.y(),ancho,alto);
            m_painter->drawEllipse(rectangle);
            update();
        }
    }
    m_configuracion = !m_configuracion;
}

void Principal::mouseMoveEvent(QMouseEvent *event)
{
    if(m_opcion == 1)
    {
        m_ptFinal = event->pos();
        QPen pincel;
        pincel.setWidth(m_ancho);
        pincel.setColor(m_color);
        m_painter->setPen(pincel);
        m_painter->drawLine(m_ptInicial,m_ptFinal);
    }
    update();
    m_ptInicial = m_ptFinal;
}

void Principal::changeEvent(QEvent *event)
{
    if(event->type()==QEvent::LanguageChange){
        ui->retranslateUi(this);
    }
}

void Principal::on_actionNuevo_triggered()
{
    nuevo();
}

void Principal::on_actionSalir_triggered()
{
    this->close();
}

void Principal::on_actionGuardar_triggered()
{
    //Crear un objeto QDir a partir del directorio del usuario
    QDir directorio = QDir::home();

    //Agregar al path absoluto del objeto un nombre por defecto del archivo
    QString pathArchivo = directorio.absolutePath();

    QString nombreArchivo = QFileDialog::getSaveFileName(
                this,tr("Guardar imagen"),pathArchivo,"Imagenes (*.png)");
    if (!nombreArchivo.isEmpty()){
        if (m_imagen->save(nombreArchivo))
            QMessageBox::information(this,tr("Guardar imagen"),tr("Archivo guardado en: ") + nombreArchivo);
        else
            QMessageBox::warning(this,tr("Guardar imagen"),tr("No se pudo guardar el archivo"));
    }
}

void Principal::on_actionColor_triggered()
{
    m_color = QColorDialog::getColor(Qt::white,this);
}

void Principal::on_actionAncho_triggered()
{
    m_ancho = QInputDialog::getInt(this, tr("Ancho del pincel"), tr("Ingrese el ancho del pincel"), m_ancho, 1, 100);
}

void Principal::on_actionLibre_triggered()
{
    m_opcion = 1;
}

void Principal::on_actionLineas_triggered()
{
    m_opcion = 2;
}

void Principal::on_actionRectangulos_triggered()
{
    m_opcion = 3;
}

void Principal::on_actionCircunferencias_triggered()
{
    m_opcion = 4;
}

void Principal::nuevo()
{
    m_imagen->fill(Qt::white);
    update();
}
