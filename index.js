const { app, BrowserWindow, ipcMain } = require('electron')
const path = require("path")
const fs = require("fs")
const os = require("os")

let win
var addon = require('bindings')('hello');

const createWindow = () => {
    win = new BrowserWindow({
        width: 800,
        height: 600,
        webPreferences: {
            nodeIntegration: false,
            contextIsolation: true,
            enableRemoteModule: false,
            preload: path.join(__dirname, 'preload.js')
        }
    })

    win.loadFile('index.html')
}

app.whenReady().then(() => {
    createWindow()

    app.on('activate', () => {
        if (BrowserWindow.getAllWindows().length === 0) {
            createWindow()
        }
    })
})

app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') {
        app.quit()
    }
})

ipcMain.on("getStateRequest", (event, args) => {
	console.log("getStateRequest called");
    try {
    	const val = addon.readValue();
        win.webContents.send("getStateResponse", val)
    } catch (err) {
        console.error(err);
    }
})

ipcMain.on("saveStateRequest", (event, val) => {
	console.log("saveStateRequest called", val); 
    try {
        addon.saveValue(val);
    } catch (err) {
        console.error(err);
    }
})

