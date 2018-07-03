import sys
import os.path
import logging
import re
import copy
import inspect

from PyQt4.QtCore import QCoreApplication

from Vispa.Main.Exceptions import exception_traceback
from Vispa.Share.BasicDataAccessor import BasicDataAccessor

import FWCore.ParameterSet.Config as cms
import six
import FWCore.GuiBrowsers.EnablePSetHistory
FWCore.GuiBrowsers.EnablePSetHistory.ACTIVATE_INSPECTION=False
from FWCore.GuiBrowsers.ConfigToolBase import ConfigToolBase
from FWCore.GuiBrowsers.editorTools import UserCodeTool

import PhysicsTools.PatAlgos.tools as tools
standardToolsDir=os.path.abspath(os.path.join(os.path.dirname(tools.__file__)))

cmsswDir="$CMSSW_BASE"
cmsswReleaseDir="$CMSSW_RELEASE_BASE"
standardConfigDir=os.path.abspath(os.path.expandvars(os.path.join(cmsswDir,"src/PhysicsTools/PatAlgos/test")))
if not os.path.exists(standardConfigDir):
    standardConfigDir=os.path.abspath(os.path.expandvars(os.path.join(cmsswReleaseDir,"src/PhysicsTools/PatAlgos/test")))

class ImportTool(ConfigToolBase):
    """ Import configuration tool """
    _label="Import"
    _defaultParameters={}
    def __init__(self):
        ConfigToolBase.__init__(self)
        self.addParameter(self._defaultParameters,'filename','/', 'The name of the configuration file')
        self._parameters=copy.deepcopy(self._defaultParameters)
        self._importCommands=[]
    def dumpPython(self):
        dump=""
        dump += "### Generated by ConfigEditor ###\n"
        find1=os.path.abspath(os.path.dirname(self._parameters['filename'].value)).find(os.path.abspath(os.path.expandvars(cmsswDir)))
        find2=os.path.abspath(os.path.dirname(self._parameters['filename'].value)).find(os.path.abspath(os.path.expandvars(cmsswReleaseDir)))
        if find1>=0 or find2>=0:
            dump += "import sys\n"
            dump += "import os.path\n"
            if find1>=0:
                dir=os.path.abspath(os.path.dirname(self._parameters['filename'].value))[find1+len(os.path.abspath(os.path.expandvars(cmsswDir)))+1:]
            else:
                dir=os.path.abspath(os.path.dirname(self._parameters['filename'].value))[find2+len(os.path.abspath(os.path.expandvars(cmsswReleaseDir)))+1:]
            dump += "sys.path.append(os.path.abspath(os.path.expandvars(os.path.join('"+cmsswDir+"','"+dir+"'))))\n"
            dump += "sys.path.append(os.path.abspath(os.path.expandvars(os.path.join('"+cmsswReleaseDir+"','"+dir+"'))))\n"
        dump += "### ------------------------- ###\n"
        if self._parameters['filename'].value!="":
            dump += "\n"
            dump += "from "+os.path.splitext(os.path.basename(self._parameters['filename'].value))[0]+" import *\n"
            dump += "\n"
            dump += "### Generated by ConfigEditor ###\n"
            dump += "if hasattr(process,'resetHistory'): process.resetHistory()\n"
            dump += "### ------------------------- ###\n"
        for command in self._importCommands:
            dump += command + "\n"
        return ("",dump)
    def apply(self,process):
        pass
    def setImportCommands(self,commands):
        self._importCommands=commands
   
class ApplyTool(ConfigToolBase):
    """Apply a tool"""
    _label="Apply tool"
    def dumpPython(self):
        return ("","")
    def apply(self,process):
        pass

class ToolDataAccessor(BasicDataAccessor):
    def __init__(self):
        logging.debug(__name__ + ": __init__")
        self._configDataAccessor=None
        self._toolList=[]
        self._importTool=ImportTool()
        self._applyTool=ApplyTool()
        self._toolModules={}
        self._processCopy=None
        self._parameterErrors={}
        
    def children(self, object):
        """ Return the children of a container object.
        """
        return ()
    
    def isContainer(self, object):
        return False

    def label(self, object):
        """ Return a string that is used as caption of an object.
        """
        if isinstance(object,type):
            directory=os.path.splitext(os.path.basename(inspect.getfile(object)))[0]
        else:
            directory=os.path.splitext(os.path.basename(inspect.getfile(type(object))))[0]
        if directory=="ToolDataAccessor":
            return object._label
        else:
            return directory+"."+object._label

    def _property(self,name,value,description,typ,allowedValues):
        if typ in [bool] and not isinstance(value, typ):
            value=False
        if typ in [int, long, float] and not isinstance(value, typ):
            value=0
        if not allowedValues is None and typ in [int,long,float,str]:
            return ("DropDown", name, value,description, False, False, allowedValues)
        elif typ in [bool]:
            return ("Boolean", name, value,description)
        elif typ in [int, long]:
            return ("Integer", name, value,description)
        elif typ in [float]:
            return ("Double", name, value,description)
        elif name=="filename":
            return ("File", name, str(value),description)
        elif name=="code":
            return ("MultilineString", name, str(value).strip("\n"),description)
        else:
            return ("String", name, str(value),description)

    def properties(self, object):
        """ Return the list of the properties of an object.
        
        Each property is represented by a tuple containing its
        type, name, value, description and readonly(True/False).
        Possible types are: 'Category','String','MultilineString','File','FileVector','Boolean','Integer','Double'.
        """
        properties=[]
        if not isinstance(object,(ImportTool,UserCodeTool,ApplyTool)):
            properties+=[("String","Tool",self.label(object),None,True)]
        if not isinstance(object,(ImportTool,UserCodeTool)):
            properties+=[("String","Description",object.description(),None,True)]
        if not isinstance(object,(UserCodeTool,ApplyTool)):
            code=object.dumpPython()
            if isinstance(code,tuple):
                code=code[1]
            properties+=[("MultilineString","code",code.strip("\n"),None,True)]
        if not isinstance(object,(ImportTool,UserCodeTool,ApplyTool)):
            properties+=[("String","comment",object._comment,None,False)]
        if len(object.getParameters().items())>0:
            properties += [("Category", "Parameters", "")]
            properties+=[self._property(value.name,value.value,value.description,value.type,object.getAllowedValues(value.name)) for key,value in object.getParameters().items()]
        return properties
    
    def setProperty(self, object, name, value, categoryName):
        """ Change the property 'name' of an object to a new value.
        """
        if self.configDataAccessor().process() and name=="code":
            # for UserCodeTool
            process=self.configDataAccessor().process()
            try:
                process.disableRecording()
                exec(value)
                process.enableRecording()
                process.resetModified()
                process.resetModifiedObjects()
            except Exception as e:
                error="Error in python code (see logfile for details):\n"+str(e)
                logging.warning(__name__ + ": setProperty: Error in python code: "+exception_traceback())
                self._parameterErrors[str(id(object))+"."+name]=error
                return error
        elif isinstance(value,str):
            # for e.g. cms.InputTag
            try:
                exec("value="+value)
            except:
                pass
        if name!="comment":
            try:
                object.setParameter(name,value)
            except Exception as e:
                error="Cannot set parameter "+name+" (see logfile for details):\n"+str(e)
                logging.warning(__name__ + ": setProperty: Cannot set parameter "+name+": "+exception_traceback())
                self._parameterErrors[str(id(object))+"."+name]=error
                return error
        elif name=="comment":
            try:
                object.setComment(value)
            except Exception as e:
                error="Cannot set comment (see logfile for details):\n"+str(e)
                logging.warning(__name__ + ": setProperty: Cannot set comment "+exception_traceback())
                self._parameterErrors[str(id(object))+"."+name]=error
                return error  
        if str(id(object))+"."+name in list(six.iterkeys(self._parameterErrors)):
            del self._parameterErrors[str(id(object))+"."+name]
        return True

    def setConfigDataAccessor(self,accessor):
        self._configDataAccessor=accessor
        self._importTool.setParameter("filename",accessor.configFile())
        if accessor.process():
            self._processCopy=copy.deepcopy(accessor.process())
            self._processCopy.resetHistory()
            return self.updateProcess(False)

    def configDataAccessor(self):
        return self._configDataAccessor

    def removeTool(self,tool):
        self._toolList.remove(tool)
        return self.updateProcess()
    
    def addTool(self,tool):
        tool.apply(self.configDataAccessor().process())
        self.configDataAccessor().setProcess(self.configDataAccessor().process())
        return True

    def updateProcess(self,cp=True):
        logging.debug(__name__ + ": updateProcess")
        if cp:
            process=copy.deepcopy(self._processCopy)
        else:
            process=self.configDataAccessor().process()
        try:
            for tool in self._toolList:
                tool.apply(process)
                if not process.checkRecording():
                    logging.error(__name__ + ": Could not apply tool: "+self.label(tool)+" (problem with enable recording flag)")
                    QCoreApplication.instance().errorMessage("Could not apply tool: "+self.label(tool)+" (problem with enable recording flag)")
                    return False
        except Exception as e:
            logging.error(__name__ + ": Could not apply tool: "+self.label(tool)+": "+exception_traceback())
            QCoreApplication.instance().errorMessage("Could not apply tool (see log file for details):\n"+str(e))
            return False
        self.configDataAccessor().setProcess(process)
        self._parameterErrors={}
        return True

    def updateToolList(self):
        logging.debug(__name__ + ": updateToolList")
        if not self.configDataAccessor().process():
            return
        history=self.configDataAccessor().process().history(True)
        self._toolList=[]
        self._toolModules={}
        importCommands=[]
        for tool,objects in history:
            if isinstance(tool,str):
                userTool=UserCodeTool()
                userTool.setParameter("code",tool)
                self._toolList+=[userTool]
            elif isinstance(tool,ConfigToolBase):
                self._toolList+=[tool]
                command=tool.dumpPython()[0]
                if command!="" and not command in importCommands:
                    importCommands+=[command]
            self._toolModules[self._toolList[-1]]=objects
        self._importTool.setImportCommands(importCommands)
        
    def topLevelObjects(self):
        objectList=[self._importTool]+self._toolList
        if self.configDataAccessor().process():
            objectList+=[self._applyTool]
        return objectList

    def toolModules(self):
        return self._toolModules

    def parameterErrors(self,object):
        return [self._parameterErrors[key] for key in list(six.iterkeys(self._parameterErrors)) if str(id(object))==key.split(".")[0]] 
