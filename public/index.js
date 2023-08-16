const leftPanel = document.querySelector('.panel.left');
const rightPanel = document.querySelector('.panel.right');
let currentTab = 0;
const files = localStorage.getItem('files')
    ? JSON.parse(localStorage.getItem('files'))
    : [
        {
            name: 'Main',
            content: 'for every I from one to ten\n' +
                '    for every J from one to I\n' +
                '        say J\n' +
                '        say space\n' +
                '    say line',
        },
    ];
window.onload = function () {
    loadTabs();
    if (files[currentTab]) {
        leftPanel.value = files[currentTab].content;
    }
};

function navigateToTutorial() {
    window.open('tutorial.html', '_blank');
}

function loadTabs() {
    const fileSystem = document.querySelector('.file-system');
    files.forEach((file, index) => {
        const tab = document.createElement('div');
        tab.classList.add('file-tab');
        tab.setAttribute('data-index', index);

        const tabName = document.createTextNode(file.name);
        tab.appendChild(tabName);

        if (index !== 0) {
            const closeButton = document.createElement('div');
            closeButton.classList.add('close-btn');
            closeButton.textContent = '×';
            closeButton.onclick = function (event) {
                event.stopPropagation();
                if (confirm('Are you sure you want to delete this file?')) {
                    tab.remove();
                    files.splice(index, 1);
                    if(currentTab === index) {
                        currentTab = 0;
                        leftPanel.value = files[currentTab].content;
                    }
                    document.querySelectorAll('.file-tab').forEach((node) => node.remove());
                    loadTabs();
                    saveToLocalStorage();
                }
            };
            tab.appendChild(closeButton);
        }

        tab.onclick = function () {
            switchTab(tab);
        };

        fileSystem.insertBefore(tab, fileSystem.lastElementChild);
    });
    fileSystem.querySelector('.file-tab').classList.add('active');
}

function createNewTab() {
    const fileName = prompt('Enter the name of the new file:');
    if (fileName && fileName.length > 20) {
        alert('File name should be less than 20 characters!');
    } else if (fileName && !files.find((file) => file.name === fileName)) {
        files.push({name: fileName, content: ''});
        saveAllFiles();
        document.querySelectorAll('.file-tab').forEach((node) => node.remove());
        loadTabs();
    } else if (files.find((file) => file.name === fileName)) {
        alert('File with this name already exists!');
    }
}

function switchTab(tabElement) {
    saveAllFiles();
    document.querySelectorAll('.file-tab').forEach((tab) => tab.classList.remove('active'));
    tabElement.classList.add('active');
    currentTab = parseInt(tabElement.getAttribute('data-index'));
    leftPanel.value = files[currentTab].content;
}

function transpile() {
    let code = leftPanel.value;
    let res = Module.transpileCode(code);
    res.output = res.output.replace(/</g, '&lt;');
    res.output = res.output.replace(/>/g, '&gt;');
    if(res.ok) {
        rightPanel.classList.remove('exception');
    }
    else {
        rightPanel.classList.add('exception');
    }
    rightPanel.innerHTML = res.output;
    saveAllFiles();
}
function runCode() {
    let code = leftPanel.value;
    let res = Module.runCode(code);
    if(res.ok) {
        rightPanel.classList.remove('exception');
    }
    else {
        rightPanel.classList.add('exception');
    }
    document.querySelector('.panel.right').innerHTML = res.output;
    saveAllFiles();
}

document.querySelector(".panel.left").addEventListener('keydown', function (e) {
    if (e.keyCode === 9) {
        e.preventDefault();

        if (document.execCommand && typeof document.execCommand === "function") {
            document.execCommand('insertText', false, '    ');
        } else {
            const selection = window.getSelection();
            const range = selection.getRangeAt(0);
            range.deleteContents();
            const node = document.createTextNode('    ');
            range.insertNode(node);

            range.setStartAfter(node);
            range.setEndAfter(node);
            selection.removeAllRanges();
            selection.addRange(range);
        }
    }
});

function saveAllFiles() {
    files[currentTab].content = leftPanel.value;
    saveToLocalStorage();
}

function saveToLocalStorage() {
    localStorage.setItem('files', JSON.stringify(files));
}