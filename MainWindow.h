#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QtGui>
#include "UpdateChecker.h"
#include "VideoTableModel.h"
#include "ProgressBarDelegate.h"
#include "VideoTableView.h"

class Downloader;

namespace Ui {
    class MainWindow;
}

/**
 * @todo paste multi-line text: get URLs from each line
 * @todo open a text file: get URLs from each line
 * @todo set automatic translation of text edit context menu (is this available?)
 * @todo at some point, add options dialog (then can remove extra options line edit)
 */
class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setSaveAndRestoreSession(bool saveAndRestore);
    bool init();

    void startDownloadFrom(QString url);
    void setExitOnSuccess(bool value) { m_exitOnSuccess = value; }

private slots:
    void chooseDestDir();

    void saveSubtitlesChoice();
    void saveSession();
    void restoreSession();

    void videoTableChanged();

    void startDownload();
    void startNextDownload();

    void reportDestFileName(QString name);
    void reportProgress(int percentage);
    void reportUnknownProgress(double secondsDownloaded);
    void downloaderOutputWritten(QString line);
    void downloadSucceeded();
    void downloadCanceled();
    void downloadFailed();

    void cancelRequested();

    void updateAvailable(QString version, QUrl url);

    void openUrl(QString url);

protected:
    void closeEvent(QCloseEvent* event);
    void showEvent(QShowEvent* event);

    /**
     * Tell URL dropping is possible
     */
    void dragEnterEvent(QDragEnterEvent* event);

    /**
     * Add URL to video list
     */
    void dropEvent(QDropEvent* event);

private:
    Ui::MainWindow *ui;

    bool m_initialized; ///< whether this object has been initialized or not

    QSettings m_settings;
    QDir m_destDir;
    QString m_destFileName;
    UpdateChecker* m_updateChecker;
    Downloader* m_downloader; // initially null
    bool m_downloadInProgress;
    bool m_resumeDownload;  ///< whether to always resume download (yle-dl --resume|-e option)
    bool m_exitOnSuccess;
    bool m_saveAndRestoreSession;   ///< whether to save & restore session (video list + states)

    void initSubtitlesComboBox();

    bool confirmCancel();

    void downloadEnded();
    void setDownloadWidgetsDisabled(bool disabled);

    void updateDestDirLabel();
    void updateVideoTableView();

    QString formatSecondsDownloaded(double secondsDownloaded);
    QNetworkAccessManager *m_networkManager;
    static QDir defaultDestDir();

    VideoTableModel* m_videoTableModel;
    VideoTableView* m_videoTableView;
    QAbstractItemView::EditTriggers m_videoTableEditTriggers;

    int m_currentlyDownloadingVideoRow; ///< current video index in m_videoTableModel
};

#endif // MAINWINDOW_H
