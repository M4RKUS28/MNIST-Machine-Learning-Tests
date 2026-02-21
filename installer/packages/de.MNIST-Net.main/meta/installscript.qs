function Component() {
   if (installer) {
      installer.installationFinished.connect(this, Component.prototype.installationFinished);
   }
}

Component.prototype.createOperations = function() {
   component.createOperations();
   if(systemInfo.productType === "windows") {

      component.addOperation("CreateShortcut", "@TargetDir@/bin/MNIST-Net.exe", "@StartMenuDir@/MNIST-Net.lnk", "description=A simple App to test my own C++ Neuronal Network Lib with MNIST");

      component.addOperation("CreateShortcut", "@TargetDir@/bin/MNIST-Net.exe", "@HomeDir@/Desktop/MNIST-Net.lnk", "description=A simple App to test my own C++ Neuronal Network Lib with MNIST");

   }
}
Component.prototype.installationFinished = function()  {
   try {
       if (installer.isInstaller()) {
           if (installer.status == QInstaller.Success) {
               if (systemInfo.productType === "windows") {
                   // Execute MNIST-Net after installation
                   if(installer.executeDetached("@TargetDir@/bin/MNIST-Net.exe", ["show"]) !== 0) {
                       print("Failed to start the application!");
                   } else {
                       print("Application started successfully.");
                   }
               } else {
                   print("Not on Windows. Skipping application execution.");
               }
           } else {
               print("Installation was not successful. Application will not be started.");
           }
       } else {
           print("Not in installer mode. Application will not be started.");
       }
   } catch(e) {
       console.log(e);
   }

}
