document.getElementById('phone').addEventListener('input', function (e) {
    let x = e.target.value.replace(/\D/g, '').substring(1);
    let formatted = '+7';

    if (x.length > 0) {
        formatted += ' (' + x.substring(0, 3);
    }
    if (x.length >= 4) {
        formatted += ') ' + x.substring(3, 6);
    }
    if (x.length >= 7) {
        formatted += '-' + x.substring(6, 8);
    }
    if (x.length >= 9) {
        formatted += '-' + x.substring(8, 10);
    }

    e.target.value = formatted;
});