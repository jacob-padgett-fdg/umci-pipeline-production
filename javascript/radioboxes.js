<script>
function getSelectedRadio(buttonGroup) {
	if (buttonGroup[0]) {
		for (var i=0; i<buttonGroup.length; i++) {
			if (buttonGroup[i].checked) { return i }
		}
	} else {
		if (buttonGroup.checked) { return 0; }
	}
	return -1;
}

function getSelectedRadioValue(buttonGroup) {
	var i = getSelectedRadio(buttonGroup);
	if (i == -1) {
		return "";
	} else {
		if (buttonGroup[i]) {
			return buttonGroup[i].value;
		} else {
			return buttonGroup.value;
		}
	}
}
</script>
