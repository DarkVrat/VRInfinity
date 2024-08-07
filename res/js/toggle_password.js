document.getElementById('toggle-password').addEventListener('click', function () {
    var passwordField = document.getElementById('password');
    var passwordFieldType = passwordField.getAttribute('type');
    if (passwordFieldType === 'password') {
        passwordField.setAttribute('type', 'text');
        this.textContent = 'Скрыть';
    } else {
        passwordField.setAttribute('type', 'password');
        this.textContent = 'Показать';
    }
});