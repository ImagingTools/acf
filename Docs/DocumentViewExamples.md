# Document/View Framework Examples

This document provides comprehensive examples for using the `idoc` and `iqtdoc` libraries in the ACF framework.

## Table of Contents

1. [Simple Single Document Application](#example-1-simple-single-document-application)
2. [Multi-Document MDI Application](#example-2-multi-document-mdi-application)
3. [Document with Undo/Redo Functionality](#example-3-document-with-undoredo-functionality)
4. [Document with Custom Meta Information](#example-4-document-with-custom-meta-information)
5. [Help System Integration](#example-5-help-system-integration)

---

## Example 1: Simple Single Document Application

This example demonstrates how to create a basic single document text editor application.

### Application Structure

```
TextEditorApp
├── Document (CTextDocumentComp)
├── View (CTextEditorGuiComp)
├── Template (CSingleDocumentTemplateComp)
├── Manager (CSingleDocumentManagerBase)
└── Main Window (CMainWindowGuiComp)
```

### Configuration File (TextEditor.acc)

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Component Type="CApplicationComp" Name="TextEditorApp">
    
    <!-- Application Info -->
    <Component Type="CStandardApplicationInfoComp" Name="ApplicationInfo">
        <Property Name="ApplicationName" Value="Simple Text Editor"/>
        <Property Name="ApplicationVersion" Value="1.0.0"/>
        <Property Name="OrganizationName" Value="MyCompany"/>
    </Component>
    
    <!-- Document Template -->
    <Component Type="idoc::CSingleDocumentTemplateComp" Name="TextDocumentTemplate">
        <Property Name="DocumentTypeId" Value="TextDocument"/>
        <Property Name="DocumentTypeName" Value="Text Document"/>
        <Property Name="DefaultDirectory" Value="."/>
        <Property Name="IsNewSupported" Value="true"/>
        <Property Name="IsEditSupported" Value="true"/>
        
        <!-- Document Factory -->
        <Component Type="idoc::CTextDocumentComp" Name="DocumentFactory">
            <Property Name="DefaultText" Value="Type your text here..."/>
        </Component>
        
        <!-- View Factory -->
        <Component Type="CTextEditorGuiComp" Name="ViewFactory"/>
        
        <!-- File Loader -->
        <Component Type="idoc::CTextFileLoaderComp" Name="DocumentLoader">
            <Property Name="FileExtensions">
                <Value>txt</Value>
                <Value>text</Value>
            </Property>
            <Property Name="TypeDescriptions">
                <Value>Text File</Value>
                <Value>Plain Text</Value>
            </Property>
        </Component>
    </Component>
    
    <!-- Document Manager -->
    <Component Type="idoc::CSingleDocumentManagerBase" Name="DocumentManager">
        <Reference Name="Template" Value="TextDocumentTemplate"/>
    </Component>
    
    <!-- Main Window -->
    <Component Type="iqtdoc::CMainWindowGuiComp" Name="MainWindow">
        <Reference Name="ApplicationInfo" Value="ApplicationInfo"/>
        <Reference Name="DocumentManager" Value="DocumentManager"/>
        <Reference Name="Application" Value="TextEditorApp"/>
        
        <Property Name="MaxRecentFiles" Value="10"/>
        <Property Name="OpenCommandVisible" Value="true"/>
        <Property Name="SaveCommandVisible" Value="true"/>
        
        <!-- Workspace for single document -->
        <Component Type="iqtdoc::CSingleDocumentWorkspaceGuiComp" Name="Workspace">
            <Reference Name="DocumentManager" Value="DocumentManager"/>
        </Component>
    </Component>
    
</Component>
```

### C++ Code for Custom Text Editor View

```cpp
// CTextEditorGuiComp.h
#pragma once

#include <QtWidgets/QPlainTextEdit>
#include <iqtgui/TGuiComponentBase.h>
#include <imod/TSingleModelObserverBase.h>
#include <idoc/ITextDocument.h>

class CTextEditorGuiComp : 
    public iqtgui::TGuiComponentBase<QPlainTextEdit>,
    public imod::TSingleModelObserverBase<idoc::ITextDocument>
{
    Q_OBJECT
    
public:
    typedef iqtgui::TGuiComponentBase<QPlainTextEdit> BaseClass;
    typedef imod::TSingleModelObserverBase<idoc::ITextDocument> BaseClass2;
    
    I_BEGIN_COMPONENT(CTextEditorGuiComp);
        I_REGISTER_INTERFACE(imod::IObserver);
    I_END_COMPONENT;
    
    // reimplemented (imod::IObserver)
    virtual bool OnModelAttached(imod::IModel* modelPtr, 
                                istd::IChangeable::ChangeSet& changeMask) override;
    virtual bool OnModelDetached(imod::IModel* modelPtr) override;
    
protected:
    // reimplemented (iqtgui::CGuiComponentBase)
    virtual void OnGuiCreated() override;
    virtual void OnGuiDestroyed() override;
    
    // reimplemented (imod::TSingleModelObserverBase)
    virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet) override;
    
private slots:
    void OnTextChanged();
    
private:
    bool m_blockUpdates;
};

// CTextEditorGuiComp.cpp
#include "CTextEditorGuiComp.h"

bool CTextEditorGuiComp::OnModelAttached(imod::IModel* modelPtr, 
                                        istd::IChangeable::ChangeSet& changeMask)
{
    if (!BaseClass2::OnModelAttached(modelPtr, changeMask))
        return false;
        
    // Load initial text from document
    idoc::ITextDocument* doc = GetModel();
    if (doc) {
        m_blockUpdates = true;
        GetWidget()->setPlainText(doc->GetText());
        m_blockUpdates = false;
    }
    
    return true;
}

bool CTextEditorGuiComp::OnModelDetached(imod::IModel* modelPtr)
{
    return BaseClass2::OnModelDetached(modelPtr);
}

void CTextEditorGuiComp::OnGuiCreated()
{
    BaseClass::OnGuiCreated();
    
    m_blockUpdates = false;
    
    // Connect text changed signal
    connect(GetWidget(), &QPlainTextEdit::textChanged, 
            this, &CTextEditorGuiComp::OnTextChanged);
}

void CTextEditorGuiComp::OnGuiDestroyed()
{
    BaseClass::OnGuiDestroyed();
}

void CTextEditorGuiComp::OnUpdate(const istd::IChangeable::ChangeSet& changeSet)
{
    // Update view when document changes
    idoc::ITextDocument* doc = GetModel();
    if (doc && !m_blockUpdates) {
        m_blockUpdates = true;
        GetWidget()->setPlainText(doc->GetText());
        m_blockUpdates = false;
    }
}

void CTextEditorGuiComp::OnTextChanged()
{
    // Update document when view changes
    if (m_blockUpdates)
        return;
        
    idoc::ITextDocument* doc = GetModel();
    if (doc) {
        m_blockUpdates = true;
        doc->SetText(GetWidget()->toPlainText());
        m_blockUpdates = false;
    }
}
```

---

## Example 2: Multi-Document MDI Application

This example shows how to create an MDI (Multiple Document Interface) application that can handle multiple documents simultaneously.

### Configuration File (MDIApp.acc)

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Component Type="CApplicationComp" Name="MDIApp">
    
    <!-- Application Info -->
    <Component Type="CStandardApplicationInfoComp" Name="ApplicationInfo">
        <Property Name="ApplicationName" Value="Multi-Document Editor"/>
        <Property Name="ApplicationVersion" Value="1.0.0"/>
    </Component>
    
    <!-- Composite Template supporting multiple document types -->
    <Component Type="idoc::CCompositeDocumentTemplateComp" Name="CompositeTemplate">
        
        <!-- Text Document Template -->
        <Component Type="idoc::CSingleDocumentTemplateComp" Name="TextTemplate">
            <Property Name="DocumentTypeId" Value="TextDocument"/>
            <Property Name="DocumentTypeName" Value="Text Document"/>
            <Property Name="IsNewSupported" Value="true"/>
            <Property Name="IsEditSupported" Value="true"/>
            
            <Component Type="idoc::CTextDocumentComp" Name="DocumentFactory"/>
            <Component Type="CTextEditorGuiComp" Name="ViewFactory"/>
            <Component Type="idoc::CTextFileLoaderComp" Name="DocumentLoader">
                <Property Name="FileExtensions">
                    <Value>txt</Value>
                </Property>
                <Property Name="TypeDescriptions">
                    <Value>Text File</Value>
                </Property>
            </Component>
            
            <!-- Add undo support -->
            <Component Type="idoc::CSerializedUndoManagerComp" Name="UndoManager">
                <Property Name="MaxBufferSize" Value="50"/>
            </Component>
        </Component>
        
        <!-- Additional document types can be added here -->
        
    </Component>
    
    <!-- Multi-Document Manager -->
    <Component Type="idoc::CMultiDocumentManagerBase" Name="DocumentManager">
        <Reference Name="Template" Value="CompositeTemplate"/>
    </Component>
    
    <!-- Main Window with MDI Workspace -->
    <Component Type="iqtdoc::CMainWindowGuiComp" Name="MainWindow">
        <Reference Name="ApplicationInfo" Value="ApplicationInfo"/>
        <Reference Name="DocumentManager" Value="DocumentManager"/>
        <Reference Name="Application" Value="MDIApp"/>
        
        <Property Name="MaxRecentFiles" Value="15"/>
        
        <!-- MDI Workspace with tabbed interface -->
        <Component Type="iqtdoc::CMultiDocumentWorkspaceGuiComp" Name="Workspace">
            <Reference Name="DocumentManager" Value="DocumentManager"/>
            <Property Name="ViewMode" Value="Tabbed"/>
            <Property Name="IsViewCloseEnabled" Value="true"/>
            <Property Name="IsTabsMovable" Value="true"/>
        </Component>
    </Component>
    
</Component>
```

### Programmatic Access to Multiple Documents

```cpp
// Get the document manager
idoc::IDocumentManager* docMgr = GetDocumentManager();

// Open multiple documents
QStringList files = {"doc1.txt", "doc2.txt", "doc3.txt"};
for (const QString& file : files) {
    QByteArray docType = "TextDocument";
    docMgr->OpenDocument(&docType, &file, true);
}

// Iterate through all open documents
int count = docMgr->GetDocumentsCount();
qDebug() << "Open documents:" << count;

for (int i = 0; i < count; ++i) {
    idoc::IDocumentManager::DocumentInfo info;
    istd::IChangeable& doc = docMgr->GetDocumentFromIndex(i, &info);
    
    qDebug() << "Document" << i << ":";
    qDebug() << "  Path:" << info.filePath;
    qDebug() << "  Type:" << info.documentTypeId;
    qDebug() << "  Dirty:" << info.isDirty;
    
    // Get views for this document
    int viewCount = docMgr->GetViewsCount(i);
    for (int j = 0; j < viewCount; ++j) {
        istd::IPolymorphic* view = docMgr->GetViewFromIndex(i, j);
        // Work with view...
    }
}

// Switch active document
istd::IPolymorphic* view = docMgr->GetViewFromIndex(1, 0);
if (view) {
    docMgr->SetActiveView(view);
}

// Close specific document
docMgr->CloseDocument(0, false); // false = ask user to save
```

---

## Example 3: Document with Undo/Redo Functionality

This example demonstrates integrating full undo/redo support into your document application.

### Configuration with Undo Manager

```xml
<Component Type="idoc::CSingleDocumentTemplateComp" Name="DocumentTemplate">
    <Property Name="DocumentTypeId" Value="MyDocument"/>
    <Property Name="IsNewSupported" Value="true"/>
    <Property Name="IsEditSupported" Value="true"/>
    
    <!-- Document must be serializable for undo to work -->
    <Component Type="MySerializableDocumentComp" Name="DocumentFactory"/>
    <Component Type="MyDocumentViewComp" Name="ViewFactory"/>
    <Component Type="MyDocumentLoaderComp" Name="DocumentLoader"/>
    
    <!-- Undo Manager Configuration -->
    <Component Type="idoc::CSerializedUndoManagerComp" Name="UndoManager">
        <Property Name="MaxBufferSize" Value="100"/>
        <!-- Optional reference to iser::IVersionInfo. Without it version dependent
             document data will be skipped during the undo state serialization. -->
        <Property Name="VersionInfo" Value="VersionInfo"/>
    </Component>
</Component>
```

### Using Undo/Redo in Code

```cpp
// Get the undo manager for active document
idoc::IDocumentManager* docMgr = GetDocumentManager();
istd::IPolymorphic* view = docMgr->GetActiveView();
istd::IChangeable* doc = docMgr->GetDocumentFromView(*view);

idoc::IUndoManager* undoMgr = docMgr->GetUndoManagerForDocument(doc);
if (!undoMgr) {
    qDebug() << "No undo manager available";
    return;
}

// Check undo availability
int undoSteps = undoMgr->GetAvailableUndoSteps();
int redoSteps = undoMgr->GetAvailableRedoSteps();

qDebug() << "Can undo" << undoSteps << "steps";
qDebug() << "Can redo" << redoSteps << "steps";

// Get descriptions of undo steps
for (int i = 0; i < undoSteps && i < 5; ++i) {
    QString desc = undoMgr->GetUndoLevelDescription(i);
    qDebug() << "Undo step" << i << ":" << desc;
}

// Perform undo
if (undoSteps > 0) {
    bool success = undoMgr->DoUndo();
    if (success) {
        qDebug() << "Undo successful";
    }
}

// Perform redo
if (redoSteps > 0) {
    bool success = undoMgr->DoRedo();
    if (success) {
        qDebug() << "Redo successful";
    }
}

// Undo multiple steps at once
int stepsToUndo = 3;
if (undoMgr->GetAvailableUndoSteps() >= stepsToUndo) {
    undoMgr->DoUndo(stepsToUndo);
}

// Check if document has unsaved changes
idoc::IDocumentStateComparator::DocumentChangeFlag flag = 
    undoMgr->GetDocumentChangeFlag();

switch (flag) {
    case idoc::IDocumentStateComparator::DCF_EQUAL:
        qDebug() << "Document unchanged";
        break;
    case idoc::IDocumentStateComparator::DCF_DIFFERENT:
        qDebug() << "Document has been modified";
        break;
    case idoc::IDocumentStateComparator::DCF_UNKNOWN:
        qDebug() << "Change status unknown";
        break;
}

// Store current state as clean (e.g., after saving)
undoMgr->StoreDocumentState();

// Clear all undo history
undoMgr->ResetUndo();
```

### Implementing Undo-Aware Document

```cpp
// Your document must implement ISerializable for undo to work
class MyDocumentComp : 
    public icomp::CComponentBase,
    virtual public iser::ISerializable
{
public:
    I_BEGIN_COMPONENT(MyDocumentComp);
        I_REGISTER_INTERFACE(iser::ISerializable);
        I_REGISTER_INTERFACE(MyIDocument);
    I_END_COMPONENT;
    
    // Implement Serialize method - this is used by undo manager
    virtual bool Serialize(iser::IArchive& archive) override
    {
        // Serialize all document state
        archive.SerializeValue("data", m_data);
        archive.SerializeValue("property1", m_property1);
        archive.SerializeValue("property2", m_property2);
        // ... serialize all document properties
        
        return archive.IsOk();
    }
    
    // When you modify document state, notify observers
    void SetData(const QString& data)
    {
        if (m_data != data) {
            m_data = data;
            NotifyObservers(); // This triggers undo snapshot
        }
    }
    
private:
    QString m_data;
    int m_property1;
    double m_property2;
};
```

---

## Example 4: Document with Custom Meta Information

This example shows how to add custom metadata to documents.

### Using Standard Meta Information

```cpp
// Get document meta info
idoc::IDocumentMetaInfo* metaInfo = GetDocumentMetaInfo();

// Set standard metadata
metaInfo->SetMetaInfo(
    idoc::IDocumentMetaInfo::MIT_TITLE, 
    "My Research Paper");
    
metaInfo->SetMetaInfo(
    idoc::IDocumentMetaInfo::MIT_AUTHOR, 
    "Dr. Jane Smith");
    
metaInfo->SetMetaInfo(
    idoc::IDocumentMetaInfo::MIT_DESCRIPTION,
    "Analysis of quantum entanglement phenomena");
    
metaInfo->SetMetaInfo(
    idoc::IDocumentMetaInfo::MIT_CREATION_TIME,
    QDateTime::currentDateTime());

// Read metadata
QString title = metaInfo->GetMetaInfo(
    idoc::IDocumentMetaInfo::MIT_TITLE).toString();
    
QString author = metaInfo->GetMetaInfo(
    idoc::IDocumentMetaInfo::MIT_AUTHOR).toString();
    
QDateTime created = metaInfo->GetMetaInfo(
    idoc::IDocumentMetaInfo::MIT_CREATION_TIME).toDateTime();

qDebug() << "Title:" << title;
qDebug() << "Author:" << author;
qDebug() << "Created:" << created.toString();
```

### Defining Custom Meta Information

```cpp
// Define custom metadata types
class MyDocumentMetaInfo : public idoc::CStandardDocumentMetaInfo
{
public:
    // Define custom meta info types
    enum CustomMetaInfoType {
        MIT_PROJECT_ID = idoc::IDocumentMetaInfo::MIT_USER,
        MIT_DEPARTMENT,
        MIT_REVIEW_STATUS,
        MIT_PRIORITY,
        MIT_TAGS
    };
    
    // Override to provide names for custom types
    virtual QString GetMetaInfoName(int metaInfoType) const override
    {
        switch (metaInfoType) {
            case MIT_PROJECT_ID:
                return tr("Project ID");
            case MIT_DEPARTMENT:
                return tr("Department");
            case MIT_REVIEW_STATUS:
                return tr("Review Status");
            case MIT_PRIORITY:
                return tr("Priority");
            case MIT_TAGS:
                return tr("Tags");
            default:
                return CStandardDocumentMetaInfo::GetMetaInfoName(metaInfoType);
        }
    }
    
    // Override to provide descriptions
    virtual QString GetMetaInfoDescription(int metaInfoType) const override
    {
        switch (metaInfoType) {
            case MIT_PROJECT_ID:
                return tr("Unique project identifier");
            case MIT_DEPARTMENT:
                return tr("Department responsible for this document");
            case MIT_REVIEW_STATUS:
                return tr("Current review status (Draft, InReview, Approved)");
            case MIT_PRIORITY:
                return tr("Document priority (Low, Medium, High, Critical)");
            case MIT_TAGS:
                return tr("Comma-separated tags for categorization");
            default:
                return CStandardDocumentMetaInfo::GetMetaInfoDescription(metaInfoType);
        }
    }
    
    // Override to provide IDs for serialization
    virtual QByteArray GetMetaInfoId(int metaInfoType) const override
    {
        switch (metaInfoType) {
            case MIT_PROJECT_ID:
                return "ProjectId";
            case MIT_DEPARTMENT:
                return "Department";
            case MIT_REVIEW_STATUS:
                return "ReviewStatus";
            case MIT_PRIORITY:
                return "Priority";
            case MIT_TAGS:
                return "Tags";
            default:
                return CStandardDocumentMetaInfo::GetMetaInfoId(metaInfoType);
        }
    }
};

// Using custom metadata
MyDocumentMetaInfo* metaInfo = GetCustomMetaInfo();

// Set custom metadata
metaInfo->SetMetaInfo(
    MyDocumentMetaInfo::MIT_PROJECT_ID, 
    "PROJ-2024-001");
    
metaInfo->SetMetaInfo(
    MyDocumentMetaInfo::MIT_DEPARTMENT, 
    "Research & Development");
    
metaInfo->SetMetaInfo(
    MyDocumentMetaInfo::MIT_REVIEW_STATUS, 
    "InReview");
    
metaInfo->SetMetaInfo(
    MyDocumentMetaInfo::MIT_PRIORITY, 
    "High");
    
metaInfo->SetMetaInfo(
    MyDocumentMetaInfo::MIT_TAGS, 
    "quantum, research, experimental");

// Read custom metadata
QString projectId = metaInfo->GetMetaInfo(
    MyDocumentMetaInfo::MIT_PROJECT_ID).toString();
QString status = metaInfo->GetMetaInfo(
    MyDocumentMetaInfo::MIT_REVIEW_STATUS).toString();

// Enumerate all metadata
idoc::IDocumentMetaInfo::MetaInfoTypes types = 
    metaInfo->GetMetaInfoTypes();
    
qDebug() << "All metadata:";
for (int type : types) {
    QString name = metaInfo->GetMetaInfoName(type);
    QString desc = metaInfo->GetMetaInfoDescription(type);
    QVariant value = metaInfo->GetMetaInfo(type);
    
    qDebug() << name << "(" << desc << "):" << value;
}
```

### Multi-Page Document with Page Metadata

```cpp
// Working with multi-page documents
idoc::IMultiPageDocument* mpDoc = GetMultiPageDocument();

// Create page with metadata
idoc::CStandardDocumentMetaInfo pageMetaInfo;
pageMetaInfo.SetMetaInfo(
    idoc::IDocumentMetaInfo::MIT_TITLE, 
    "Chapter 1: Introduction");
pageMetaInfo.SetMetaInfo(
    idoc::IDocumentMetaInfo::MIT_DESCRIPTION,
    "Introduction to the topic");

// Insert page with metadata
istd::IChangeable* page1 = mpDoc->InsertPage(&pageMetaInfo, nullptr, -1);

// Access page metadata later
for (int i = 0; i < mpDoc->GetPagesCount(); ++i) {
    const idoc::IDocumentMetaInfo* pageMeta = mpDoc->GetPageMetaInfo(i);
    if (pageMeta) {
        QString title = pageMeta->GetMetaInfo(
            idoc::IDocumentMetaInfo::MIT_TITLE).toString();
        qDebug() << "Page" << i << ":" << title;
    }
}

// Access document-level metadata
const idoc::IDocumentMetaInfo& docMeta = mpDoc->GetDocumentMetaInfo();
QString docTitle = docMeta.GetMetaInfo(
    idoc::IDocumentMetaInfo::MIT_TITLE).toString();
```

---

## Example 5: Help System Integration

This example demonstrates how to integrate comprehensive help documentation into your application.

### Configuration with Doxygen Help

```xml
<Component Type="iqtdoc::CMainWindowGuiComp" Name="MainWindow">
    <Reference Name="ApplicationInfo" Value="ApplicationInfo"/>
    <Reference Name="DocumentManager" Value="DocumentManager"/>
    
    <!-- HTML Help Viewer with Doxygen Provider -->
    <Component Type="iqtdoc::CHtmlHelpGuiComp" Name="HelpViewer">
        <Component Type="iqtdoc::CDoxygenHelpFileProviderComp" Name="HelpFileProvider">
            <Property Name="HelpDirectory" Value="doc/html"/>
            <Property Name="IndexFileName" Value="index.html"/>
            <Property Name="ClassPrefix" Value="class"/>
            <Property Name="NamespacePrefix" Value="namespace"/>
        </Component>
    </Component>
    
</Component>
```

### Configuration with Static HTML Help

```xml
<Component Type="iqtdoc::CHtmlHelpGuiComp" Name="HelpViewer">
    <Component Type="iqtdoc::CStaticHelpFileProviderComp" Name="HelpFileProvider">
        <Property Name="HelpDirectory" Value="help"/>
        <Property Name="HelpFileExtension" Value="html"/>
        <Property Name="DefaultHelpFile" Value="index.html"/>
    </Component>
</Component>
```

### Using Help System in Code

```cpp
// Get help viewer
idoc::IHelpViewer* helpViewer = GetHelpViewer();

// Show help for a specific class/topic
helpViewer->ShowHelp("IDocumentManager", nullptr);
helpViewer->ShowHelp("CSingleDocumentTemplateComp", nullptr);

// Show help for current widget/object
QWidget* currentWidget = GetCurrentWidget();
helpViewer->ShowHelp("", currentWidget);

// Check help availability before showing
QString topic = "UndoRedoSystem";
double quality = helpViewer->GetHelpQuality(topic, nullptr);

if (quality > 0.0) {
    qDebug() << "Help available with quality:" << quality;
    
    if (quality >= 0.7) {
        // High quality help available
        helpViewer->ShowHelp(topic, nullptr);
    } else {
        // Lower quality help, maybe show warning
        QMessageBox::StandardButton reply = QMessageBox::question(
            nullptr,
            "Help",
            "Only generic help is available. Show it anyway?",
            QMessageBox::Yes | QMessageBox::No);
            
        if (reply == QMessageBox::Yes) {
            helpViewer->ShowHelp(topic, nullptr);
        }
    }
} else {
    QMessageBox::information(
        nullptr,
        "Help",
        "No help available for this topic.");
}
```

### Context-Sensitive Help

```cpp
// Implement context-sensitive help in custom widgets
class MyWidget : public QWidget
{
public:
    MyWidget(QWidget* parent = nullptr) : QWidget(parent)
    {
        // Enable help button in window
        setWindowFlags(windowFlags() | Qt::WindowContextHelpButtonHint);
    }
    
protected:
    // Override to provide custom help
    virtual void contextMenuEvent(QContextMenuEvent* event) override
    {
        QMenu menu(this);
        
        QAction* helpAction = menu.addAction("Help");
        connect(helpAction, &QAction::triggered, [this]() {
            ShowHelp();
        });
        
        menu.exec(event->globalPos());
    }
    
    void ShowHelp()
    {
        idoc::IHelpViewer* helpViewer = GetHelpViewer();
        if (helpViewer) {
            // Show help for this widget
            helpViewer->ShowHelp("MyWidget", this);
        }
    }
};

// Add F1 shortcut for help
QShortcut* helpShortcut = new QShortcut(QKeySequence("F1"), mainWindow);
connect(helpShortcut, &QShortcut::activated, []() {
    idoc::IHelpViewer* helpViewer = GetHelpViewer();
    if (helpViewer) {
        // Show help for currently focused widget
        QWidget* focusWidget = QApplication::focusWidget();
        helpViewer->ShowHelp("", focusWidget);
    }
});
```

### Creating Custom Help File Provider

```cpp
// Custom help provider that maps contexts to URLs
class CCustomHelpFileProviderComp : 
    public icomp::CComponentBase,
    virtual public idoc::IHelpFileProvider
{
public:
    I_BEGIN_COMPONENT(CCustomHelpFileProviderComp);
        I_REGISTER_INTERFACE(idoc::IHelpFileProvider);
        I_REGISTER_INTERFACE(idoc::IHelpInfoProvider);
        I_ASSIGN(m_helpUrlAttrPtr, "HelpBaseUrl", "Base URL for help", 
                 true, "https://mycompany.com/help/");
    I_END_COMPONENT;
    
    // Map context to help file path or URL
    virtual QString GetHelpFilePath(
        const QString& contextText, 
        const istd::IPolymorphic* contextObjectPtr) const override
    {
        QString baseUrl = *m_helpUrlAttrPtr;
        
        // Map specific contexts to help pages
        if (contextText == "IDocumentManager") {
            return baseUrl + "document_manager.html";
        } else if (contextText == "FileOperations") {
            return baseUrl + "file_operations.html";
        } else if (contextText.startsWith("idoc::")) {
            // Generate URL from class name
            QString className = contextText.mid(6); // Remove "idoc::"
            return baseUrl + "idoc/" + className.toLower() + ".html";
        }
        
        // Check object type if provided
        if (contextObjectPtr) {
            // Use RTTI or ACF type system to determine object type
            // and map to appropriate help page
        }
        
        // Default/fallback help
        return baseUrl + "index.html";
    }
    
    virtual double GetHelpQuality(
        const QString& contextText,
        const istd::IPolymorphic* contextObjectPtr) const override
    {
        QString helpPath = GetHelpFilePath(contextText, contextObjectPtr);
        
        // Check if help file exists
        if (QFile::exists(helpPath)) {
            // Return higher quality for specific help
            if (contextText.startsWith("idoc::") || 
                contextText.startsWith("iqtdoc::")) {
                return 1.0; // Specific class help
            }
            return 0.7; // Generic help available
        }
        
        return 0.0; // No help available
    }
    
private:
    I_ATTR(QString, m_helpUrlAttrPtr);
};
```

---

## Additional Tips

### Best Practices

1. **Document Naming**: Use meaningful document type IDs that reflect the document content
2. **File Extensions**: Register all relevant file extensions for your document type
3. **Undo Management**: Set appropriate MaxBufferSize based on document complexity
4. **Recent Files**: Configure MaxRecentFiles based on user workflow
5. **Metadata**: Use standard metadata types when possible for better interoperability

### Common Patterns

#### Checking Document Dirty State

```cpp
bool IsDocumentDirty(idoc::IDocumentManager* docMgr, int docIndex = -1)
{
    if (docIndex < 0) {
        // Check active document
        istd::IPolymorphic* view = docMgr->GetActiveView();
        if (!view) return false;
        
        idoc::IDocumentManager::DocumentInfo info;
        docMgr->GetDocumentFromView(*view, &info);
        return info.isDirty;
    } else {
        // Check specific document
        idoc::IDocumentManager::DocumentInfo info;
        docMgr->GetDocumentFromIndex(docIndex, &info);
        return info.isDirty;
    }
}
```

#### Save All Documents

```cpp
bool SaveAllDocuments(idoc::IDocumentManager* docMgr)
{
    bool allSaved = true;
    int count = docMgr->GetDocumentsCount();
    
    for (int i = 0; i < count; ++i) {
        idoc::IDocumentManager::DocumentInfo info;
        docMgr->GetDocumentFromIndex(i, &info);
        
        if (info.isDirty) {
            bool saved = docMgr->SaveDocument(i);
            if (!saved) {
                allSaved = false;
                qDebug() << "Failed to save:" << info.filePath;
            }
        }
    }
    
    return allSaved;
}
```

#### Prompt to Save on Close

```cpp
bool PromptToSaveBeforeClose(idoc::IDocumentManager* docMgr)
{
    // Use built-in functionality
    bool ignored = false;
    bool success = docMgr->SaveDirtyDocuments(false, &ignored);
    
    if (ignored) {
        // User canceled the operation
        return false;
    }
    
    return success;
}
```

---

## See Also

- [idoc namespace documentation](../html/namespaceidoc.html)
- [iqtdoc namespace documentation](../html/namespaceiqtdoc.html)
- [IDocumentManager interface](../html/classidoc_1_1_i_document_manager.html)
- [ACF Component Framework](../html/group__component_framework.html)
