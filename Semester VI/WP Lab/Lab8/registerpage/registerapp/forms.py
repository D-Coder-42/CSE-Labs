from django import forms

class RegisterForm(forms.Form):
    username = forms.CharField(
        label="Username",
        max_length=100,
        required=True,
        widget=forms.TextInput(attrs={'placeholder': 'Enter Username'})
    )
    password = forms.CharField(
        label="Password",
        max_length=100,
        required=False,
        widget=forms.PasswordInput(attrs={'placeholder': 'Enter Password'})
    )
    email = forms.EmailField(
        label="Email",
        required=False,
        widget=forms.EmailInput(attrs={'placeholder': 'Enter Email'})
    )
    contact_number = forms.CharField(
        label="Contact Number",
        max_length=15,
        required=False,
        widget=forms.TextInput(attrs={'placeholder': 'Enter Contact Number'})
    )
